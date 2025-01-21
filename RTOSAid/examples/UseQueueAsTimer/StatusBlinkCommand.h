/*
 * StatusBlinkCommand.h
 *
 *  Created on: Nov 21, 2023
 *      Author: Eric Mintz
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
 * A command message that orders the status blink task to blink a status
 * code or to go dark (i.e. not blink at all).
 */

#ifndef STATUSBLINKCOMMAND_H_
#define STATUSBLINKCOMMAND_H_

#include "Arduino.h"

typedef struct {
  // Time to hold the status LED on (illuminated) in milliseconds
  uint32_t on_time_millis;

  // Time to hold the status LED off (dark) in milliseconds
  uint32_t off_time_millis;

  // Time to wait between the statuc sequence of on/off blinks in milliseconds
  uint32_t wait_between_blink_sequence_millis;

  // Number of times to blink, with 0 meaning don't blink.
  uint16_t status_code;

} StatusBlinkCommand;

#endif /* STATUSBLINKCOMMAND_H_ */
