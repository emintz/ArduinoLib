/*
 * AssailantAction.h
 *
 *  Created on: Nov 23, 2023
 *      Author: Eric Mintz
 *
 * A TaskAction that beats up the TargetClass.
 *
 * Copyright (C) 2023 Eric Mintz
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
 *
 */

#ifndef ASSAILANTACTION_H_
#define ASSAILANTACTION_H_

#include "Arduino.h"
#include "TaskAction.h"

class TargetClass;

class AssailantAction : public TaskAction {
  TargetClass *target;
  uint32_t delay;
  uint8_t led_pin;

public:
  /**
   * Creates an instance that invokes the specified target
   *
   * Parameters:
   *
   * Name    Contents
   * ------- ------------------------------------------------------------------
   * target  The target to invoke
   * delay   How long for the target to hold its mutex, in milliseconds
   * led_pin The GPIO that drives an LED for the action to illuminate while
   *         the target holds the lock.
   */
  AssailantAction(
      TargetClass *target,
      uint32_t delay,
      uint8_t led_pin);
  virtual ~AssailantAction();

  /**
   * The loop that assaults the target
   */
  virtual void run(void);
};

#endif /* ASSAILANTACTION_H_ */
