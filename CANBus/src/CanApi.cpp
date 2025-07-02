/*
 * CanApi.cpp
 *
 *  Created on: May 28, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
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

#include "Arduino.h"
#include "CanApi.h"
#include "CanBusMaps.h"

#include "driver/gpio.h"

CanApi::CanApi(
    uint8_t rx_pin,
    uint8_t tx_pin,
    CanBusSpeed speed,
    uint8_t bus,
    CanBusMode can_bus_mode) :
        rx_pin(rx_pin),
        tx_pin(tx_pin),
        speed(speed),
        bus(bus),
        mode(twai_mode_t::TWAI_MODE_NORMAL),
        h_twai(NULL) {
}

CanApi::~CanApi() {
}

void CanApi::begin(void) {
  Serial.printf("CanApi::begin() compiled on %s at %s.\n",
      __DATE__, __TIME__);
}

const char *CanApi::to_cstr(esp_err_t error_code) {
  return CanBusMaps::INSTANCE.to_c_string(error_code);
}

const char *CanApi::to_cstr(twai_state_t state) {
  return CanBusMaps::INSTANCE.to_c_string(state);
}

CanBusStatus CanApi::bus_status(void) {
  twai_status_info_t status_info;
  memset(&status_info, 0, sizeof(status_info));
  CanBusStatus result = CanBusStatus::CORRUPT;
  if (!h_twai) {
    result = CanBusStatus::DOWN;
  } else {
    switch (twai_get_status_info_v2(h_twai, &status_info)) {
      case ESP_OK:
        switch (status_info.state) {
          case TWAI_STATE_STOPPED:
            result = CanBusStatus::STOPPED;
            break;
          case TWAI_STATE_RUNNING:
            result = CanBusStatus::ACTIVE;
            break;
          case TWAI_STATE_BUS_OFF:
            result = CanBusStatus::ERROR_HALT;
            break;
          case TWAI_STATE_RECOVERING:
            result = CanBusStatus::RECOVERING;
        }
        break;

      case ESP_ERR_INVALID_STATE:
        result = CanBusStatus::DOWN;
      break;

      default:
        break;
    }
  }
  return result;
}

esp_err_t CanApi::clear_receive_queue(void) {
  return twai_clear_receive_queue_v2(h_twai);
}

esp_err_t CanApi::clear_transmit_queue(void) {
  return twai_clear_transmit_queue_v2(h_twai);
}

esp_err_t CanApi::install(void) {
  twai_general_config_t general_config =
      TWAI_GENERAL_CONFIG_DEFAULT(
          static_cast<gpio_num_t>(tx_pin),
          static_cast<gpio_num_t>(rx_pin),
          mode);
  general_config.controller_id = bus;
  const twai_timing_config_t& timing_config =
      CanBusMaps::INSTANCE.to_twai_speed(speed);
  twai_filter_config_t filter_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  return twai_driver_install_v2(&general_config, &timing_config, &filter_config, &h_twai);
}

esp_err_t CanApi::read_alerts(uint32_t& alerts, int timeout_ms) {
  return twai_read_alerts_v2(h_twai, &alerts, pdMS_TO_TICKS(timeout_ms));
}

esp_err_t CanApi::read_status_info(twai_status_info_t& info) {
  return twai_get_status_info_v2(h_twai, &info);
}

esp_err_t CanApi::receive(twai_message_t& message, int timeout_ms) {
  return twai_receive_v2(h_twai, &message, pdMS_TO_TICKS(timeout_ms));
}

esp_err_t CanApi::reconfigure_alerts(uint32_t alerts) {
  return twai_reconfigure_alerts_v2(h_twai, alerts, NULL);
}

esp_err_t CanApi::recover_if_bus_off(void) {
  twai_status_info_t bus_status;
  esp_err_t esp_status = read_status_info(bus_status);
  if (ESP_OK == esp_status && TWAI_STATE_BUS_OFF == bus_status.state) {
    esp_status = twai_initiate_recovery_v2(h_twai);
  }
  return esp_status;
}

esp_err_t CanApi::send(const twai_message_t& message, int timeout_ms) {
  return twai_transmit_v2(h_twai, &message, pdMS_TO_TICKS(timeout_ms));
}

esp_err_t CanApi::start(void) {
  return twai_start_v2(h_twai);
}

esp_err_t CanApi::stop(void) {
  esp_err_t result = twai_stop_v2(h_twai);
  if (ESP_OK == result) {
    h_twai = NULL;
  }
  return result;
}

esp_err_t CanApi::uninstall(void) {
  esp_err_t status = twai_driver_uninstall_v2(h_twai);
  if (ESP_OK == status) {
    h_twai = NULL;
  }
  return status;
}
