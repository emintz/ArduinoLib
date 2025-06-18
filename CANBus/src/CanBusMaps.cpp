/*
 * CanBusMaps.cpp
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

#include "CanBusMaps.h"

CanBusMaps::CanBusMaps() {
  bus_mode_map[CanBusMode::LURK] = TWAI_MODE_LISTEN_ONLY;
  bus_mode_map[CanBusMode::NORMAL] = TWAI_MODE_NORMAL;
  bus_mode_map[CanBusMode::SELF_TEST] = TWAI_MODE_NO_ACK;

  bus_speed_map[CanBusSpeed::BPS_25K] = TWAI_TIMING_CONFIG_25KBITS();
  bus_speed_map[CanBusSpeed::BPS_50K] = TWAI_TIMING_CONFIG_50KBITS();
  bus_speed_map[CanBusSpeed::BPS_100K] = TWAI_TIMING_CONFIG_100KBITS();
  bus_speed_map[CanBusSpeed::BPS_125K] = TWAI_TIMING_CONFIG_125KBITS();
  bus_speed_map[CanBusSpeed::BPS_250K] = TWAI_TIMING_CONFIG_250KBITS();
  bus_speed_map[CanBusSpeed::BPS_500K] = TWAI_TIMING_CONFIG_500KBITS();
  bus_speed_map[CanBusSpeed::BPS_800K] = TWAI_TIMING_CONFIG_800KBITS();
  bus_speed_map[CanBusSpeed::BPS_1M] = TWAI_TIMING_CONFIG_1MBITS();

  error_code_name[ESP_OK] = "ESP_OK";
  error_code_name[ESP_ERR_INVALID_ARG] = "ESP_ERR_INVALID_ARG";
  error_code_name[ESP_ERR_NO_MEM] = "ESP_ERR_NO_MEM";
  error_code_name[ESP_ERR_INVALID_STATE] = "ESP_ERR_INVALID_STATE";
  error_code_name[ESP_ERR_TIMEOUT] = "ESP_ERR_TIMEOUT";
  error_code_name[ESP_FAIL] = "ESP_FAIL";
  error_code_name[ESP_ERR_NOT_SUPPORTED] = "ESP_ERR_NOT_SUPPORTED";
  error_code_name[ESP_ERR_NO_MEM] = "ESP_ERR_NO_MEM";

  op_status_map[ESP_OK] = CanBusOpStatus::SUCCEEDED;
  op_status_map[ESP_ERR_TIMEOUT] = CanBusOpStatus::TIMEOUT;
  op_status_map[ESP_ERR_INVALID_ARG] = CanBusOpStatus::INVALID_ARGUMENT;
  op_status_map[ESP_ERR_INVALID_STATE] = CanBusOpStatus::UNAVAILABLE;
  op_status_map[ESP_ERR_NO_MEM] = CanBusOpStatus::MEMORY_FULL;
  op_status_map[ESP_ERR_NOT_SUPPORTED] = CanBusOpStatus::CANNOT_SEND;
  op_status_map[ESP_FAIL] = CanBusOpStatus::TRANSMIT_FAILED;

  op_status_to_string[CanBusOpStatus::SUCCEEDED] = "SUCCEEDED";
  op_status_to_string[CanBusOpStatus::UNAVAILABLE] = "UNAVAILABLE";
  op_status_to_string[CanBusOpStatus::INVALID_ARGUMENT] = "INVALID_ARGUMENT";
  op_status_to_string[CanBusOpStatus::TIMEOUT] = "TIMEOUT";
  op_status_to_string[CanBusOpStatus::TRANSMIT_FAILED] = "TRANSMIT_FAILED";
  op_status_to_string[CanBusOpStatus::CANNOT_SEND] = "CANNOT_SEND";
  op_status_to_string[CanBusOpStatus::MEMORY_FULL] = "MEMORY_FULL";
  op_status_to_string[CanBusOpStatus::INVALID_STATE] = "INVALID_STATE";
  op_status_to_string[CanBusOpStatus::FAILED] = "FAILED";
  op_status_to_string[CanBusOpStatus::UNKNOWN] = "UNKNOWN";

  twai_state_map[TWAI_STATE_STOPPED] = "TWAI_STATE_STOPPED";
  twai_state_map[TWAI_STATE_RUNNING] = "TWAI_STATE_RUNNING";
  twai_state_map[TWAI_STATE_BUS_OFF] = "TWAI_STATE_BUS_OFF";
  twai_state_map[TWAI_STATE_RECOVERING] = "TWAI_STATE_RECOVERING";

  bus_status_to_string[CanBusStatus::DOWN] = "DOWN";
  bus_status_to_string[CanBusStatus::STOPPED] = "STOPPED";
  bus_status_to_string[CanBusStatus::ACTIVE] = "ACTIVE";
  bus_status_to_string[CanBusStatus::ERROR_HALT] = "ERROR_HALT";
  bus_status_to_string[CanBusStatus::RECOVERING] = "RECOVERING";
  bus_status_to_string[CanBusStatus::CORRUPT] = "CORRUPT";
}

CanBusMaps::~CanBusMaps() {
}

const CanBusMaps CanBusMaps::INSTANCE;

const char *CanBusMaps::to_c_string(CanBusStatus status) const {
  const auto iter = bus_status_to_string.find(status);
  return iter == bus_status_to_string.end()
      ? "Unknown bus status"
      : iter->second;
}

const char *CanBusMaps::to_c_string(esp_err_t error_code) const {
  const auto iter = error_code_name.find(error_code);
  return iter == error_code_name.end()
      ? "Unknown error code"
      : iter->second;
}

CanBusOpStatus CanBusMaps::to_op_status(esp_err_t error_code) const {
  const auto iter =
      op_status_map.find(error_code);
  return iter == op_status_map.end()
      ? CanBusOpStatus::UNKNOWN
      : iter->second;
}

const char *CanBusMaps::to_c_string(twai_state_t state) const {
  const auto iter = twai_state_map.find(state);
  return iter == twai_state_map.end()
      ? "UNKNOWN BUS STATE"
      : iter->second;
}
