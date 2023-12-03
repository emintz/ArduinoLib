/*
 * PinChangeAction.h
 *
 *  Created on: Dec 2, 2023
 *      Author: Eric Mintz
 *
 * A TaskAction that waits for a notification and changes a specified GPIO
 * level when notified.
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

#ifndef PINCHANGEACTION_H_
#define PINCHANGEACTION_H_

#include "Arduino.h"
#include "TaskAction.h"

class PinChangeAction : public TaskAction {
  uint8_t pin_no;
  uint8_t level;
public:
  PinChangeAction(
      uint8_t pin_no);
  virtual ~PinChangeAction();

  /**
   * When notified, changes the specified LED from LOW to HIGH or from HIGH to
   * LOW.
   */
  virtual void run(void);
};

#endif /* PINCHANGEACTION_H_ */
