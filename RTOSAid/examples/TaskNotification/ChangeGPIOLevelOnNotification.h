/*
 * ChangeGPIOLevelOnNotification.h
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 *
 * Task that, when notified, changes protected GPIO pin level from HIGH to
 * LOW or vice versa.
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
 */

#ifndef CHANGEGPIOLEVELONNOTIFICATION_H_
#define CHANGEGPIOLEVELONNOTIFICATION_H_

#include "Arduino.h"

#include "TaskAction.h"

/**
 * Action that changes the level on a specified pin each time it
 * is notified.
 */
class ChangeGPIOLevelOnNotification : public TaskAction {

  const uint8_t gpio_pin;
  uint8_t level;

public:

  /**
   * Constructor that binds the created instance to the target GPIO pin
   *
   * Name     Contents
   * -------- -----------------------------------------------------------
   * gpio_pin The pin to manipulate. Must be a valid GPIO pin number
   */
  ChangeGPIOLevelOnNotification(
      uint8_t gpio_pin);
  virtual ~ChangeGPIOLevelOnNotification();

  /**
   * The loop that changes the pin level when notified.
   */
  virtual void run(void);
};

#endif /* CHANGEGPIOLEVELONNOTIFICATION_H_ */
