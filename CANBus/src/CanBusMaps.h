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

#include "CanEnumerations.h"

#include <map>

#include "driver/twai.h"

class CanBusMaps final {

  std::map<CanBusMode, twai_mode_t> bus_mode_map;
  std::map<CanBusSpeed, twai_timing_config_t> bus_speed_map;
  std::map<esp_err_t, const char *> error_code_name;
  std::map<esp_err_t,CanBusOpStatus> op_status_map;
  std::map<twai_state_t, const char *> twai_state_map;
public:


  CanBusMaps();
  ~CanBusMaps();

  twai_mode_t to_twai_mode(CanBusMode can_bus_mode) {
    return bus_mode_map[can_bus_mode];
  }

  twai_timing_config_t& to_twai_speed(CanBusSpeed can_bus_speed) {
    return bus_speed_map[can_bus_speed];
  }

  const char *to_c_string(esp_err_t error_code);

  const char *to_c_string(twai_state_t state);

  CanBusOpStatus to_op_status(esp_err_t error_code);
};

#endif /* LIBRARIES_CANBUS_SRC_CANBUSMAPS_H_ */
