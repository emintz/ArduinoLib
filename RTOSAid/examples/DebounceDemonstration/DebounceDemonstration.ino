/**
 * DebounceDemonstration.ino
 *
 *  Created on: Dec 5, 2023
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
 * A button change detector that debounces switch makes and breaks. Like
 * the Switch Bounce Detector sketch, the debounce demonstration watches for
 * a button to open and close, and counts the changes. The
 * CounterActionAndFunction class, which contains the application logic,
 * is an exact copy of its counterpart in the switch bounce detector
 * sketch. The only change is to substitute its GpioChangeDetector with a
 * GpioDebouncer.
 */
#include "Arduino.h"

#include "CounterActionAndFunction.h"

#include "GpioDebouncer.h"
#include "TaskWithAction.h"

#define TEST_PUSH_BUTTON_PIN 27

static CounterActionAndFunction bounce_counter(TEST_PUSH_BUTTON_PIN);

static uint8_t bounce_counter_stack[2048];
static TaskWithAction bounce_counter_task(
    "Counter",
    3,
    &bounce_counter,
    bounce_counter_stack,
    sizeof(bounce_counter_stack));

static GpioDebouncer debouncer(
    TEST_PUSH_BUTTON_PIN,
    10000,
    "DBTask",
    3,
    "DBTimer",
    &bounce_counter);

void halt_and_catch_fire(const char* reason) {
  Serial.printf("Halting because %s.\n", reason);
  for (;;) {
    vTaskDelay(portMAX_DELAY);
  }
}


void setup() {
  Serial.begin(115200);
  Serial.printf(
      "GPIO switch debounce example compiled on %s at %s.\n",
      __DATE__,
      __TIME__);

  pinMode(TEST_PUSH_BUTTON_PIN, INPUT_PULLUP);

  if (!GpioChangeService.begin()) {
    halt_and_catch_fire("Could not start the GPIO change service.");
  }

  if (!bounce_counter.begin()) {
    halt_and_catch_fire("Could not start the bounce counter.");
  }

  if (!bounce_counter_task.start()) {
    halt_and_catch_fire("Could not start the counter task.");
  }

  if (!debouncer.start()) {
    halt_and_catch_fire("Could not start the debouncer.");
  }

  Serial.println("Setup completed.");
}

void loop() {
  // Because the bounce counter task does all the work, there's nothing
  // to do here.
  vTaskDelay(portMAX_DELAY);
}
