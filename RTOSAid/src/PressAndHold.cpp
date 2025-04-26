/*
 * CheckForReset.cpp
 *
 *  Created on: Feb 5, 2024
 *      Author: Eric Mintz
 *
 * Copyright (C) 2024 Eric Mintz
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

#include "PressAndHold.h"


PressAndHold::PressAndHold(
    uint8_t pin,
    uint32_t press_timeout,
    uint32_t hold_duration) :
      pin(pin),
      press_timeout(press_timeout),
      hold_duration(hold_duration) {
  state = State::WAITING_FOR_PRESS;
  active_timeout = hold_duration;
  time_in_state_millis = 0;
  current_time_millis = millis();
}

PressAndHold::~PressAndHold() {
}

bool PressAndHold::wait_for_press_and_hold() {
  bool button_held = false;
  pinMode(pin, INPUT_PULLUP);
  active_timeout = press_timeout;

  for (;;) {
    Event received_event = digitalRead(pin) == LOW ? Event::PRESSED : Event::RELEASED;
    State maybe_new_state = TRANSITION_TABLE[(int)state][(int)received_event];
    if (maybe_new_state != State::STATE_COUNT) {
      switch (state = maybe_new_state) {
        case State::WAITING_FOR_PRESS:
          // Nothing to do
          break;
        case State::BUTTON_PRESSED:
          active_timeout = hold_duration;
          time_in_state_millis = 0;
          button_held = true;
          break;
        case State::HOLDING:
          // Nothing to do.
          break;
        case State::BUTTON_RELEASED:
          active_timeout = press_timeout;
          time_in_state_millis = 0;
          button_held = false;
          break;
        case State::TIME_EXPIRED:
          // Terminal state, nothing to do.
          break;
        case State::STATE_COUNT:
          // Should never happen
          break;
      }
    }

    // If the current deadline has expired, return.
    uint32_t now = millis();
    uint32_t increment = (current_time_millis ^ now) & 1;
    current_time_millis = now;
    time_in_state_millis += increment;
    if (active_timeout <= time_in_state_millis) {
      state = State::TIME_EXPIRED;
      return button_held;
    }

    vTaskDelay(1);
  }
}
