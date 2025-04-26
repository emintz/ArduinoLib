/*
 * CheckForReset.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Eric Mintz
 *
 * Waits for a user to press and hold a button for a specified duration.
 * For the press and hold to be detected, the user must press the button
 * within a specified timeout. The button must be wired between the specified
 * pin and ground.
 *
 * This class blocks. Use it during setup only.
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

#ifndef PRESSANDHOLD_H_
#define PRESSANDHOLD_H_

#include "Arduino.h"

class PressAndHold {

  enum class Event {
    PRESSED = 0,
    RELEASED = 1,
    EVENT_COUNT = 3,
  };

  enum class State {
    WAITING_FOR_PRESS = 0,  // Waiting for the user to press the button
    BUTTON_PRESSED = 1,  // User has just pressed the button (HIGH to LOW transition).
    HOLDING = 2,  // User is holding the button closed (LOW to HIGH transition).
    BUTTON_RELEASED = 3,  // User has released the button.
    TIME_EXPIRED = 4,  // Timeout has expired. Terminal state
    STATE_COUNT = 5,  // Must be last. Transitions into this state are ignored.
  };

  const State TRANSITION_TABLE[(int)State::STATE_COUNT][(int)Event::EVENT_COUNT] =
    {
      {  // WAITING_FOR_PRESS
        State::BUTTON_PRESSED,  // Pressed
        State::STATE_COUNT,  // Released
      },
      {  // BUTTON_PRESSED
         State::HOLDING,  // Pressed
         State::BUTTON_RELEASED,  // Released
      },
      {  // HOLDING
         State::HOLDING,   // Pressed
         State::BUTTON_RELEASED, // Released
      },
      {  // BUTTON_RELEASED
         State::BUTTON_PRESSED,  // Pressed
         State::WAITING_FOR_PRESS,  // Released
      },
      {  // TIME_EXPIRED
          State::STATE_COUNT,  // Pressed
          State::STATE_COUNT,  // Released
      },
    };

  const uint8_t pin;
  const uint32_t press_timeout;
  const uint32_t hold_duration;

  State state;
  uint32_t active_timeout;
  uint32_t current_time_millis;
  uint32_t time_in_state_millis;

public:
  /**
   * Configure a PressAndHold instance
   *
   * Parameters:
   *
   * +===============+========================================================+
   * | Name          | Contents                                               |
   * +===============+========================================================+
   * | pin           | GPIO pin connected to the button.                      |
   * +---------------+--------------------------------------------------------+
   * | press_timeout | Milliseconds to wait for the user to press the button  |
   * +---------------+--------------------------------------------------------+
   * | hold_duration | Milliseconds to hold the button                        |
   * +---------------+--------------------------------------------------------+
   */
  PressAndHold(
      uint8_t pin,
      uint32_t press_timeout,
      uint32_t hold_duration);
  virtual ~PressAndHold();

  /**
   * Wait for the user to press and hold the button or for the press timeout
   * to expire.
   *
   * Returns: true if the user pressed and held the button for the specified
   * time, false otherwise.
   */
  bool wait_for_press_and_hold(void);
};

#endif /* PRESSANDHOLD_H_ */
