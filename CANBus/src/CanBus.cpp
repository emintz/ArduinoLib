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

#import "CanBus.h"

#import "Arduino.h"
#import "CanBusMaps.h"
#import "CanPayload.h"
#import "CanPayloadHandler.h"

#import "MutexLock.h"

static esp_err_t print_status(esp_err_t status) {
  Serial.printf("Status = %d (%s).\n",
      static_cast<int>(status), CanBusMaps::INSTANCE.to_c_string(status));
  return status;
}

static void dump_state(CanApi& can_api) {
  twai_status_info_t status_info;
  memset(&status_info, 0, sizeof(status_info));
  Serial.println("Fetching bus status for state dump.");
  esp_err_t status_ret = can_api.read_status_info(status_info);
  print_status(status_ret);
  if (ESP_OK == status_ret) {
    Serial.printf(
        "Bus status: %s, pending sends: %d, errors: %d, pending transmits; "
            "%d, errors: %d, bus errors: %d.\n",
            CanBusMaps::INSTANCE.to_c_string(status_info.state),
        static_cast<int>(status_info.msgs_to_tx),
        static_cast<int>(status_info.tx_error_counter),
        static_cast<int>(status_info.msgs_to_rx),
        static_cast<int>(status_info.rx_error_counter),
        static_cast<int>(status_info.bus_error_count));
  }
}

CanBusOpStatus CanBus::drain_input_queue(void) {
  CanBusOpStatus result = CanBusOpStatus::SUCCEEDED;
  twai_status_info_t status_info;
  do {
    memset(&status_info, 0, sizeof(status_info));
    result = CanBusMaps::INSTANCE.to_op_status(
        can_api.read_status_info(status_info));
    if (result == CanBusOpStatus::SUCCEEDED && 0 < status_info.msgs_to_rx) {
      vTaskDelay(pdMS_TO_TICKS(5));
    }
  } while (result == CanBusOpStatus::SUCCEEDED && 0 < status_info.msgs_to_rx);
}

CanBusOpStatus CanBus::maybe_start_alert_task(
      CanAlertHandlers& alert_handlers) {
  CanBusOpStatus result = CanBusOpStatus::SUCCEEDED;
  if (uint32_t active_alerts = alert_handlers.get_active_alerts()) {
    Serial.println("Starting the alert task.");
    result = CanBusMaps::INSTANCE.to_op_status(
        can_api.reconfigure_alerts(active_alerts));
    if (CanBusOpStatus::SUCCEEDED == result) {
      alert_action = std::make_unique<CanAlertAction>(
          alert_handlers, *this, *(receive_action.get()));
      alert_task = std::make_unique<TaskWithActionH>(
          "alert-handling",
          20,
          alert_action.get(),
          8192);
      if (alert_task->start()) {
        result = CanBusOpStatus::SUCCEEDED;
        Serial.println("Alert task is running \\o/");
      } else {
        result = CanBusOpStatus::FAILED;
        alert_task.reset();
        alert_action.reset();
        Serial.println("Alert task startup failed :-(");
      }
    }
  } else {
    Serial.println("No alerts configured; not starting alert task.");
  }
  return result;
}

CanBusInitStatus CanBus::really_init(void) {
    CanBusInitStatus start_status = CanBusInitStatus::FAILED;
  esp_err_t install_result = can_api.install();

  if (install_result == ESP_OK) {
    start_status = CanBusInitStatus::SUCCEEDED;
  }
  dump_state(can_api);
  return start_status;
}

CanBusDeinitStatus CanBus::really_deinit(void) {
  esp_err_t stop_status = can_api.uninstall();
  print_status(stop_status);
  return stop_status == ESP_OK
      ? CanBusDeinitStatus::SUCCEEDED
      : CanBusDeinitStatus::FAILED;
}

CanBusOpStatus CanBus::really_start(
    CanPayloadHandler& payload_handler,
    CanAlertHandlers& alert_handlers) {
  auto result = start_bus();
  if (CanBusOpStatus::SUCCEEDED == result) {
    result = start_receive_task(payload_handler);
  }

  if (CanBusOpStatus::SUCCEEDED == result) {
    result = maybe_start_alert_task(alert_handlers);
  }

  if (CanBusOpStatus::SUCCEEDED == result) {
    set_receive_status(CanReceiveStatus::RECEIVING);
  }

  return result;
}

CanBusOpStatus CanBus::really_stop(void) {
  CanBusOpStatus result = CanBusMaps::INSTANCE.to_op_status(can_api.stop());
  if (CanBusOpStatus::SUCCEEDED == result) {
    result = drain_input_queue();
  }
  if (CanBusOpStatus::SUCCEEDED == result) {
    shut_down_receive_task();
    result = wait_for_bus_shutdown();
  }
  if (CanBusOpStatus::SUCCEEDED == result) {
    set_receive_status(CanReceiveStatus::DOWN);
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
  CanBusOpStatus status =
      CanBusMaps::INSTANCE.to_op_status(can_api.start());
  Serial.printf("CanBus::start_bus returns: %s.\n", CanBusMaps::INSTANCE.to_c_string(status));
  return status;
}

CanBusOpStatus CanBus::start_receive_task(CanPayloadHandler& handler) {
  Serial.println("Starting the receive task.");
  CanBusOpStatus result = CanBusOpStatus::FAILED;
  receive_action = std::make_unique<CanPayloadAction>(
      *this, handler);
  receive_task = std::make_unique<TaskWithActionH>(
      "can-receive",
      19,
      receive_action.get(),
      8192);
  if (receive_task->start()) {
    result = CanBusOpStatus::SUCCEEDED;
    Serial.println("Start task is running \\o/");
  }
  if (CanBusOpStatus::SUCCEEDED != result) {
    receive_task.reset();
    receive_action.reset();
    Serial.println("Receive task startup failed :-(");
  }
  Serial.printf("CanBus::start_receive_task returns: %s.\n",
      CanBusMaps::INSTANCE.to_c_string(result));
  return result;
}

CanBusOpStatus CanBus::really_transmit(
    const twai_message_t& message,
    int timeout_ms) {
//  Serial.printf(
//      "In really_transmit, extd: %d, self: %d, flags: 0X%x, id: %d, length: %d.\n",
//      static_cast<int>(message.extd),
//      static_cast<int>(message.self),
//      static_cast<int>(message.flags),
//      static_cast<int>(message.identifier),
//      static_cast<int>(message.data_length_code));
  CanBusOpStatus status = CanBusMaps::INSTANCE.to_op_status(
    can_api.send(message, timeout_ms));
//  Serial.println("Message sent.");
  return status;
}

CanBusOpStatus CanBus::wait_for_bus_shutdown(void) {
  esp_err_t esp_status;
  twai_status_info_t status_info;
  bool keep_waiting = false;
  do {
    memset(&status_info, 0, sizeof(status_info));
    esp_status = can_api.read_status_info(status_info);
    keep_waiting =
        ESP_OK == esp_status
        && TWAI_STATE_STOPPED != status_info.state;
    if (keep_waiting) {
      vTaskDelay(pdMS_TO_TICKS(5));
    }
  } while (keep_waiting);
  return CanBusMaps::INSTANCE.to_op_status(esp_status);
}

CanBus::CanBus(
    uint8_t receive_pin,
    uint8_t transmit_pin,
    CanBusSpeed bits_per_second,
    CanBusMode mode,
    CanBusNumber bus_number) :
        can_api(
            receive_pin,
            transmit_pin,
            bits_per_second,
            static_cast<uint8_t>(bus_number),
            mode),
        receive_status(CanReceiveStatus::DOWN) {
  status_mutex.begin();
}

CanBus::~CanBus(void) {
}


 void CanBus::begin(void) {
   CanApi::begin();
 }

CanBusInitStatus CanBus::init(void) {
  Serial.printf("At CanBus::init(), bus status is %s,\n",
      CanBusMaps::INSTANCE.to_c_string(can_api.bus_status()));
  return can_api.bus_status() == CanBusStatus::DOWN
      ? really_init()
      : CanBusInitStatus::ALREADY_UP;
}

CanBusDeinitStatus CanBus::deinit(void) {
  CanBusDeinitStatus status = CanBusDeinitStatus::NOT_STOPPED;
  switch (can_api.bus_status()) {
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

CanBusOpStatus CanBus::recover_if_bus_off(void) {
  return CanBusMaps::INSTANCE.to_op_status(can_api.recover_if_bus_off());
}

CanBusOpStatus CanBus::start(
    CanPayloadHandler& payload_handler,
    CanAlertHandlers& alert_handlers) {
  CanBusOpStatus result = CanBusOpStatus::INVALID_STATE;
  if (can_api.bus_status() == CanBusStatus::STOPPED) {
    result = really_start(payload_handler, alert_handlers);
  }
  return result;
}

CanBusOpStatus CanBus::stop(void) {
  CanBusOpStatus result = CanBusOpStatus::INVALID_STATE;
  switch (can_api.bus_status()) {
    case CanBusStatus::STOPPED:  // Already stopped, nothing to do
      result = CanBusOpStatus::SUCCEEDED;
      break;
    case CanBusStatus::ACTIVE:  // Running, shut the bus down
      result = really_stop();
      break;
    default:  // Cannot stop in the current state. Result code already set.
      break;
  }
  return result;
}

CanBusOpStatus CanBus::transmit(const CanPayload& payload, int timeout_ms) {
  return can_api.bus_status() == CanBusStatus::ACTIVE
      ? really_transmit(payload.as_twai_message(), timeout_ms)
      : CanBusOpStatus::INVALID_STATE;
}
