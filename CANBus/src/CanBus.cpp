/*
 * CanBus.cpp
 *
 *  Created on: May 24, 2025
 *      Author: Eric Mintz
*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "CanBus.h"

#include "Arduino.h"
#include "CanBusMaps.h"
#include "CanPayloadHandler.h"

#include "MutexLock.h"

CanBusMaps can_bus_maps;

static esp_err_t print_status(esp_err_t status) {
  Serial.printf("Status = %d (%s).\n",
      static_cast<int>(status), can_bus_maps.to_c_string(status));
  return status;
}

static void dump_state(twai_handle_t h_twai) {
  twai_status_info_t status_info;
  memset(&status_info, 0, sizeof(status_info));
  Serial.println("Fetching bus status for state dump.");
  esp_err_t status_ret = twai_get_status_info_v2(h_twai, &status_info);
  print_status(status_ret);
  if (ESP_OK == status_ret) {
    Serial.printf(
        "Bus status: %s.\n", can_bus_maps.to_c_string(status_info.state));
  }
}

CanBusInitStatus CanBus::really_init(
    int bus_number,
    uint8_t receive_pin,
    uint8_t transmit_pin,
    CanBusSpeed bits_per_second,
    CanBusMode mode) {
    CanBusInitStatus start_status = CanBusInitStatus::FAILED;

  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
      static_cast<gpio_num_t>(receive_pin),
      static_cast<gpio_num_t>(transmit_pin),
      can_bus_maps.to_twai_mode(mode));
  g_config.controller_id = bus_number;
  twai_timing_config_t t_config = can_bus_maps.to_twai_speed(bits_per_second);
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  esp_err_t install_result = print_status(
      twai_driver_install_v2(&g_config, &t_config, &f_config, &h_twai));

  if (install_result == ESP_OK) {
    bus_status = CanBusStatus::STOPPED;
    start_status = CanBusInitStatus::SUCCEEDED;
  }
  dump_state(h_twai);;
  return start_status;
}

CanBusDeinitStatus CanBus::really_deinit(void) {
  esp_err_t stop_status = twai_driver_uninstall_v2(h_twai);
  print_status(stop_status);
  if (stop_status == ESP_OK) {
    bus_status = CanBusStatus::DOWN;
    h_twai = NULL;
  }
  return stop_status == ESP_OK
      ? CanBusDeinitStatus::SUCCEEDED
      : CanBusDeinitStatus::FAILED;
}

CanBusOpStatus CanBus::really_start(CanPayloadHandler& handler) {
  auto result = start_receive_task(handler);
  if (CanBusOpStatus::SUCCEEDED == result) {
    result = start_bus();
  }

  if (CanBusOpStatus::SUCCEEDED == result) {
    bus_status = CanBusStatus::ACTIVE;
    receive_status = CanReceiveStatus::RECEIVING;
  } else {
    receive_task.reset();
    receive_action.reset();
  }

  return result;
}

void CanBus::shut_down_receive_task(void) {
  if (receive_task) {
    receive_task->stop();
  }
  receive_task.reset();
  receive_action.reset();
}

void CanBus::set_receive_status(CanReceiveStatus receive_status)  {
  MutexLock lock(status_mutex);
  this->receive_status = receive_status;
}

CanBusOpStatus CanBus::start_bus(void) {
  return can_bus_maps.to_op_status(twai_start_v2(h_twai));

}

CanBusOpStatus CanBus::start_receive_task(CanPayloadHandler& handler) {
  CanBusOpStatus result = CanBusOpStatus::FAILED;
  receive_action = std::make_unique<CanPayloadAction>(
      *this, handler);
  receive_task = std::make_unique<TaskWithActionH>(
      "can-receive",
      20,
      receive_action.get(),
      4096);
  if (receive_task->start()) {
    result = CanBusOpStatus::SUCCEEDED;
  } else {
    shut_down_receive_task();
  }
  return result;
}

CanBus::CanBus() :
    h_twai(NULL),
    bus_status(CanBusStatus::DOWN),
    receive_status(CanReceiveStatus::DOWN) {
  status_mutex.begin();
}

CanBus::~CanBus() {
}

CanBusInitStatus CanBus::init(
    int bus_number,
    uint8_t receive_pin,
    uint8_t transmit_pin,
    CanBusSpeed bits_per_second,
    CanBusMode mode) {
  return bus_status == CanBusStatus::DOWN
      ? really_init(
          bus_number, receive_pin, transmit_pin, bits_per_second, mode)
      : CanBusInitStatus::ALREADY_UP;
}

CanBusDeinitStatus CanBus::deinit(void) {
  CanBusDeinitStatus status = CanBusDeinitStatus::NOT_STOPPED;
  switch (bus_status) {
    case CanBusStatus::STOPPED:
      status = really_deinit();
      break;
    case CanBusStatus::DOWN:
      status = CanBusDeinitStatus::ALREADY_DOWN;
      break;
    default:
      break;
  }
  return status;
}

CanReceiveStatus CanBus::get_receive_status(void) {
  MutexLock lock(status_mutex);
  return receive_status;
}



CanBusOpStatus CanBus::start(CanPayloadHandler& handler) {
  CanBusOpStatus result = CanBusOpStatus::INVALID_STATE;
  if (bus_status == CanBusStatus::STOPPED) {
    result = really_start(handler);
  }
  return result;
}

CanBusOpStatus CanBus::stop(void) {
  CanBusOpStatus result = CanBusOpStatus::INVALID_STATE;
  if (CanReceiveStatus::DOWN != receive_status) {
    result = can_bus_maps.to_op_status(twai_stop_v2(h_twai));
    if (CanBusOpStatus::SUCCEEDED == result) {
      twai_status_info_t bus_status_info;
      memset(&bus_status, 0, sizeof(bus_status));
      do {
        twai_get_status_info_v2(h_twai, &bus_status_info);
        if (0 < bus_status_info.msgs_to_rx) {
          vTaskDelay(pdMS_TO_TICKS(2));
        }
      } while (0 < bus_status_info.msgs_to_rx);
      shut_down_receive_task();
      receive_status = CanReceiveStatus::DOWN;
      bus_status = CanBusStatus::STOPPED;
    }
  }
  return result;
}
