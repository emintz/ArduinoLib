/*
 * CanAlertAction.cpp
 *
 *  Created on: Jun 3, 2025
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

#import "CanAlertAction.h"
#import "CanApi.h"
#import "CanAlertAction.h"
#import "CanAlertHandlers.h"
#import "CanBus.h"
#import "CanPayloadAction.h"

CanAlertAction::CanAlertAction(
    CanAlertHandlers& handlers,
    CanBus& can_bus,
    CanPayloadAction& payload_action) :
        handlers(handlers),
        can_bus(can_bus),
        payload_action(payload_action) {
}

void CanAlertAction::run(void) {
  uint32_t alerts;
  while(payload_action.run_stop_flag()) {
    CanApi& can_api = can_bus.get_can_api();
    while (payload_action.run_stop_flag()) {
      if (ESP_OK == can_api.read_alerts(alerts, 20)) {
        handlers.forward_alerts(alerts, can_bus);
      }
    }
  }
  stop();
}
