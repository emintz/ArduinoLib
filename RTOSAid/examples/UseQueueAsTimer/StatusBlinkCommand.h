/*
 * StatusBlinkCommand.h
 *
 *  Created on: Nov 21, 2023
 *      Author: Eric Mintz
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
