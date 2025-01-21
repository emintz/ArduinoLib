/**
 * BlinkAction.cpp
 *
 *  Created on: Nov 16, 2023
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
 */

#include <BlinkyAction.h>

#include "Arduino.h"

#define BUILTIN_LED_PIN 2  // Might be different on your board

BlinkyAction::BlinkyAction() {
}

BlinkyAction::~BlinkyAction() {
}

void BlinkyAction::run(void) {
  // Set up: set the builtin LED pin to OUTPUT.
  pinMode(BUILTIN_LED_PIN, OUTPUT);

  // Endless task loop
  for (;;) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay_millis(500);  // Can also use delay()
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay_millis(500);
  }
}
