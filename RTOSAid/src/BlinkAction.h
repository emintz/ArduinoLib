/*
 * BlinkAction.h
 *
 *  Created on: Jan 19, 2025
 *      Author: Eric Mintz
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
 *
 * A task action that blinks an LED, a common requirement for
 * embedded applications.
 */

#ifndef BLINKACTION_H_
#define BLINKACTION_H_

#include "TaskAction.h"

class BlinkAction: public TaskAction {

  const uint16_t led_pin;
  uint16_t number_of_flashes;
  uint16_t on_time_ms;
  uint16_t off_time_ms;
  TickType_t inter_group_wait_ms;

public:
  /**
   * Initialize the task and set the default blink characteristics.
   *
   * Parameters            Contents
   * --------------------- ------------------------------------------------
   * led_pin               Powers the LED to blink
   * number_of_flashes     The number of times to flash.
   * on_time_ms            How long the output pin (and by extension the LED)
   *                       stays HIGH (on) in milliseconds.
   * off_time_ms           How long the output pin stays LOW (off) in
   *                       milliseconds.
   * inter_group_wait_ms   The wait time between between flash groups in
   *                       milliseconds
   */
  BlinkAction(
      const uint16_t led_pin,
      uint16_t number_of_flashes,
      uint16_t on_time_ms,
      uint16_t off_time_ms,
      uint16_t inter_group_wait_ms);
  virtual ~BlinkAction();

  /**
   * Runs the blink task. Note that the LED will start
   * blinking as configured at construction.
   */
  virtual void run(void);

  virtual void blink_off(void);

  void blink_on(
      uint16_t number_of_flashes,
      uint16_t on_time_ms,
      uint16_t off_time_ms,
      uint16_t inter_group_wait_ms);
};

#endif /* BLINKACTION_H_ */
