/*
 * GpioBounceCounter.cpp
 *
 *  Created on: Dec 3, 2023
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
 */

#include "CounterActionAndFunction.h"
#include "PinDefinitions.h"

static uint8_t yellow_led_level = LOW;

CounterActionAndFunction::CounterActionAndFunction() :
    count(0) {
}

CounterActionAndFunction::~CounterActionAndFunction() {
}

void CounterActionAndFunction::apply(void) {
  // Make sure that run() cannot access count while we increment it.
  // Hold the mutex for the minimum possible time.
  {
    MutexLock lock(mutex);
    ++count;
  }
  digitalWrite(RED_LED_PIN, digitalRead(TEST_PUSH_BUTTON_PIN));
}

bool CounterActionAndFunction::begin(void) {
  return mutex.begin();
}

void CounterActionAndFunction::run(void) {
  uint32_t current_count = 0;
  for (;;) {
    delay_millis(15000);
    // Make sure that apply cannot change the count while we access it
    // by locking the mutex. To minimize the time we hold the lock, we
    // simply copy the count to a local variable for use after we release
    // the mutex.
    {
      MutexLock lock(mutex);
      current_count = count;
    }
    Serial.printf("Count is %lu.\n", current_count);
    yellow_led_level = (LOW == yellow_led_level) ? HIGH : LOW;
    digitalWrite(YELLOW_LED_PIN, yellow_led_level);
  }
}
