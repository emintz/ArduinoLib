/*
 * CanBusMaps.h
 *
 *  Created on: May 24, 2025
 *      Author: Eric Mintz
 *
 * Maps CanBus enumerations to their counterparts in the ESP32
 * CAN bus library.
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

#ifndef LIBRARIES_CANBUS_SRC_CANBUSMAPS_H_
#define LIBRARIES_CANBUS_SRC_CANBUSMAPS_H_

#import "CanApi.h"
#import "CanEnumerations.h"

#import <map>

#import "driver/twai.h"

class CanBusMaps final {

  std::map<CanBusMode, twai_mode_t> bus_mode_map;
  std::map<CanBusSpeed, twai_timing_config_t> bus_speed_map;
  std::map<esp_err_t, const char *> error_code_name;
  std::map<esp_err_t, CanBusOpStatus> op_status_map;
  std::map<CanBusOpStatus, const char *> op_status_to_string;
  std::map<twai_state_t, const char *> twai_state_map;
  std::map<CanBusStatus, const char *> bus_status_to_string;
public:
  CanBusMaps();
  ~CanBusMaps();

  static const CanBusMaps INSTANCE;

  inline const char *to_c_string(CanBusOpStatus status) const {
    const auto iter = op_status_to_string.find(status);
    return iter->second;
  }

  const char *to_c_string(CanBusStatus status) const;

  const char *to_c_string(esp_err_t error_code) const;

  const char *to_c_string(twai_state_t state) const;

  CanBusOpStatus to_op_status(esp_err_t error_code) const;

  inline twai_mode_t to_twai_mode(CanBusMode can_bus_mode) {
    return bus_mode_map[can_bus_mode];
  }

  inline const twai_timing_config_t& to_twai_speed(
      CanBusSpeed can_bus_speed) const {
    const auto iter = bus_speed_map.find(can_bus_speed);
    return iter->second;
  }
};

#endif /* LIBRARIES_CANBUS_SRC_CANBUSMAPS_H_ */
