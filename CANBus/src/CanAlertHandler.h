/*
 * CanAlertHandler.h
 *
 *  Created on: Jun 2, 2025
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

#include "Arduino.h"

class CanAlertAction;  // Provides run/stop status
class CanBus;

class CanAlertHandler {
public:
  /*
   * Handle CAN bus alerts
    *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * can_bus           The bus that issued the alert
   * alerts            The alert mask containing only the bits handled
   *                   by this handler. Irrelevant bits will all be 0
   *                   no matter their value set by the bus.
   * payload_handler   The payload handler, which provides run/stop
   *                   status.
   */
  virtual void operator()(CanBus &can_bus, uint32_t alerts) = 0;
};
