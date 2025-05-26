/*
 * CanPayloadAction.cpp
 *
 *  Created on: May 26, 2025
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

#include "CanPayloadAction.h"

#include "CanBus.h"
#include "CanPayloadHandler.h"

CanPayloadAction::CanPayloadAction(
    CanBus& bus,
    CanPayloadHandler& handler) :
        bus(bus),
        handler(handler) {
}

CanPayloadAction::~CanPayloadAction() {
}

void CanPayloadAction::run(void) {
  bus.set_receive_status(CanReceiveStatus::RECEIVING);
  twai_message_t message;
  while (true) {
    switch (bus.receive(message)) {
      case ESP_OK: {
          CanPayload payload(message);
          handler(bus, payload);
        }
        break;
      case ESP_ERR_TIMEOUT:
        // Nothing received. This is not an error, just
        // try again.
        break;
      default:
        bus.set_receive_status(CanReceiveStatus::PANIC);
        stop();
    }
  }
}
