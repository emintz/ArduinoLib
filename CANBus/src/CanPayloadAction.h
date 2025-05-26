/*
 * CanPayloadAction.h
 *
 *  Created on: May 26, 2025
 *      Author: Eric Mintz
 *
 * Task action that handles incoming messages.
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

#ifndef LIBRARIES_CANBUS_CANPAYLOADACTION_H_
#define LIBRARIES_CANBUS_CANPAYLOADACTION_H_

#include "CanPayload.h"
#include "CanPayloadHandler.h"
#include "TaskAction.h"

#include "driver/twai.h"

class CanBus;

class CanPayloadAction : public TaskAction {
  CanBus& bus;
  CanPayloadHandler& handler;
public:
  CanPayloadAction(
      CanBus& bus,
      CanPayloadHandler& handler);
  virtual ~CanPayloadAction();

  virtual void run(void);
};

#endif /* LIBRARIES_CANBUS_CANPAYLOADACTION_H_ */
