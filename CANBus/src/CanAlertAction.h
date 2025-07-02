/*
 * CanAlertAction.h
 *
 *  Created on: Jun 3, 2025
 *      Author: Eric Mintz
 *
 * Action for the alert processor task. The action retrieves alerts
 * and forwards them to the appropriate alert handler(s).
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

#ifndef LIBRARIES_CANBUS_CANALERTACTION_H_
#define LIBRARIES_CANBUS_CANALERTACTION_H_

#include "Arduino.h"

#include "TaskAction.h"
#include "driver/twai.h"

class CanAlertHandlers;
class CanApi;
class CanBus;
class CanPayloadAction;

class CanAlertAction final : public TaskAction {
  CanAlertHandlers& handlers;
  CanBus& can_bus;
  CanPayloadAction& payload_action;

public:
  CanAlertAction(
      CanAlertHandlers& handlers,
      CanBus& can_bus,
      CanPayloadAction& payload_action);

  virtual void run(void) override;
};

#endif /* LIBRARIES_CANBUS_CANALERTACTION_H_ */
