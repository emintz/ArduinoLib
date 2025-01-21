#include "Arduino.h"

/**
 * GpioChangeDetector demonstration
 *
 *  Created on: Jan 19, 2025
 *      Author: Eric Mintz
 *
 * Demonstration of GpioDebouncer use
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
 * Change the built-in LED's state on GPIO level change. Toggle
 * the change via a push button. This should be OK because LED
 * illumination is relatively insensitive to switch bounce.
 */

#include "CounterActionAndFunction.h"

#define BUILTIN_LED_PIN 2
#define RED_LED_PIN 13
#define YELLOW_LED_PIN 14
#define GREEN_LED_PIN 15
#define BLUE_LED_PIN 16
#define WHITE_LED_PIN 23
#define TEST_SIGNAL_OUT_PIN 25
#define TEST_SIGNAL_IN_PIN 26
#define TEST_PUSH_BUTTON_PIN 27

static CounterActionAndFunction level_change_action;

void setup() {
  Serial.begin(115200);
  pinMode(TEST_PUSH_BUTTON_PIN, INPUT_PULLUP);
  if (!level_change_action.begin()) {
    Serial.println("Could not start the GPIO change handler.");
  }
}

void loop() {
}
