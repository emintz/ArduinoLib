/**
 * BlinkAction.h
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 *
 *  Action that blinks the ESP32 development board's built in LED
 *  every second. The blink is 500 milleseconds on, 500 milliseconds
 *  off.
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

#ifndef BLINKACTION_H_
#define BLINKACTION_H_

#include "TaskAction.h"
#include "TaskWithAction.h"

class BlinkAction: public TaskAction {
public:
  BlinkAction();
  virtual ~BlinkAction();

  /**
   * The code that blinks the LED.
   */
  virtual void run(void);
};

#endif /* BLINKACTION_H_ */
