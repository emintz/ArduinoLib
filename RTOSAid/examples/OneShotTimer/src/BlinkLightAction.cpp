/*
 * BlinkLightTask.cpp
 *
 *  Created on: Oct 13, 2025
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
 */

#include "BlinkLightAction.h"

#define BUILTIN_LED_PIN 2

/**
 * Since Serial.read() provides one character at a time, the wait time
 * must be decoded character by character. A state machine is a good
 * implementation.
 *
 * We partition characters into three classes:
 *
 *   1. Digits
 *   2. New-line
 *   3. Everything else a.k.a. Other.
 *
 * We want to ignore leading and trailing non-digits and terminate
 * when a new-line arrives. The following state machine does this:
 *
 *                         +<----------<+
 *                         |            ^
 *                         V            |
 *                 +---------------+    ^
 *                 | Leading in:   |    | Other
 *        +<------<| discard input |>---+
 *        |        +---------------+
 *  New   |                V
 *        |                | Digit
 *  Line  V                +<----------<+
 *        |                |            ^
 *        |                V            |
 *        |  New   +---------------+    ^
 *        |  Line  | Decoding:     |    | Digit
 *        +<------<| build value   |>---+
 *        |        +---------------+
 *        V                V
 *        |                | Other
 *        |                +<----------<+
 *        |                |            ^
 *        |                V            | Digit
 *        |        +---------------+    ^ Other
 *        |        | Leading out:  |    |
 *        V        | ignore input  |>---+
 *        |        +---------------+
 *        |                V
 *        |                |
 *        V                | New-Line
 *        |                V
 *        |        +---------------+
 *        |        | Leading out:  |
 *        +------> | ignore input  |
 *                 | (Terminal)    |
 *                 +---------------+
 */
enum class InputType {
  NUMERIC,            // '0' .. '9'
  NEW_LINE,           // '\n'
  OTHER,              // Everything else, including '\r'
  NUMBER_OF_INPUTS,   // MUST be last
};

enum class State {
  LEADING_IN,       // At or after the line start and before the first numeral
  DECODING,         // Building the numeric value
  RUNNING_OUT,      // After the last numeral and before new-line
  TERMINATED,       // At end of line
  NUMBER_OF_STATES, // MUST be last
};

static const State TRANSITION_TABLE
    [static_cast<size_t>(State::NUMBER_OF_STATES)]
    [static_cast<size_t>(InputType::NUMBER_OF_INPUTS)] =
    {
      {  // LEAD_IN
         State::DECODING,     // NUMERIC
         State::TERMINATED,   // NEW_LINE
         State::LEADING_IN,      // OTHER
      },
      {  // DECODING
         State::DECODING,     // NUMERIC
         State::TERMINATED,   // NEW_LINE
         State::RUNNING_OUT,  // OTHER
      },
      {  // RUNNING_OUT
         State::RUNNING_OUT,  // NUMERIC
         State::TERMINATED,   // NEW_LINE
         State::RUNNING_OUT,  // OTHER
      },
      {  // TERMINATED
         State::DECODING,    // NUMERIC
         State::TERMINATED,  // NEW_LINE
         State::LEADING_IN,     // OTHER
      },
    };

/**
 * Classifies an input character
 *
 * Parameters
 *
 * Name            Contents
 * --------------- ----------------------------------------------------------
 * input_char      Character to classify
 *
 * Return: the character type
 */
static InputType type_of(int input_char) {
  if ('0' <= input_char && input_char <= '9') {
    return InputType::NUMERIC;
  } else if ('\n' == input_char) {
    return InputType::NEW_LINE;
  }
  return InputType::OTHER;
}

/**
 * Poll the serial input for the next available input character. Ignore
 * "no input" indications and cede control between attempts so the
 * global watchdog timer has a chance to reset.
 *
 * Returns: the entered character
 */
int read_next_char(void) {
  int the_input = -1;
  while (-1 == the_input) {
    vTaskDelay(1);
    the_input = Serial.read();
  }
  return the_input;
}

/**
 * Read the delay from serial input.
 *
 * Returns: the desired delay in milliseconds
 */
static int read_delay(void) {
  int delay = 0;
  State state = State::LEADING_IN;

  while (state != State::TERMINATED) {
    int input_char = read_next_char();
    switch (state = TRANSITION_TABLE
        [static_cast<size_t>(state)]
        [static_cast<size_t>(type_of(input_char))]) {
      case State::LEADING_IN:
        break;
      case State::DECODING:
        delay = 10*delay + (input_char - '0');
        break;
      case State::RUNNING_OUT:
        break;
      case State::TERMINATED:
        break;
    }
  }

  return delay;
}

BlinkLightAction::~BlinkLightAction() {
}
void BlinkLightAction::run(void) {
  vTaskDelay(1);  // Give setUp() time to complete.
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  digitalWrite(BUILTIN_LED_PIN, LOW);
  State state = State::LEADING_IN;
  int input = 0;
  Serial.println("Starting blink loop.");
  for (;;) {
     Serial.print("Millisecond delay: ");
     int delay_ms = read_delay();
     Serial.printf("Waiting for %d milliseconds.\n", delay_ms);
     Serial.flush();
     auto start_time = millis();
     digitalWrite(BUILTIN_LED_PIN, HIGH);
     timer_->start_ms(delay_ms);
     wait_for_notification();
     digitalWrite(BUILTIN_LED_PIN, LOW);
     Serial.printf("Waited for %lu milliseconds.\n", millis() - start_time);
     vTaskDelay(1);
  }
}
