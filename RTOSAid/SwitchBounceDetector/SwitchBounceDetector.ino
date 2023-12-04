/**
 * SwitchBounceDetector.ino
 *
 *  Created on: Dec 3, 2023
 *      Author: Eric Mintz
 *
 * Detects switch bounce.
 *
 * A VoidFunction that counts its invocations and a TaskWithAction that
 * displays the count and changes the yellow LED state every 15 seconds.
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
 * The sketch counts the number of times a switch opens and closes, and
 * prints the accumulated count every 15 seconds. The switch is wired
 * active LOW, meaning that the voltage across the switch is HIGH when
 * it's open and LOW when it's closed. The sketch applies the switch voltage
 * to the red LED, so it is on when the switch is open and off when it is
 * closed.
 *
 * The sketch also toggles (i.e. turns off if on or on if off) when it
 * prints the count.
 *
 * To use the sketch:
 *
 * 1. Upload it and wait for the count (which should be 0) to print.
 * 2. Close the switch and wait for the count to print
 * 3. Open the switch and wait for the count to print.
 *
 * Repeat this for as long as you please. If the switch doesn't bounds, the
 * count will increase by 1 each time. When it bounces, the increase will be
 * greater.
 *
 * Some switches don't bounce very much. If you want to see severe bounce,
 * connect a wire to TEST_PUSH_BUTTON_PIN and another to ground, and touch
 * them together.
 */
#include "Arduino.h"

#include "Arduino.h"
#include "CounterActionAndFunction.h"

#include "GpioChangeDetector.h"
#include "TaskWithAction.h"

#define BUILTIN_LED_PIN 2
#define RED_LED_PIN 13
#define YELLOW_LED_PIN 14
#define GREEN_LED_PIN 15
#define BLUE_LED_PIN 16
#define WHITE_LED_PIN 23
#define TEST_SIGNAL_OUT_PIN 25
#define TEST_SIGNAL_IN_PIN 26
#define TEST_PUSH_BUTTON_PIN 27

static CounterActionAndFunction bounce_counter;

static uint8_t bounce_counter_stack[2048];
static TaskWithAction bounce_counter_task(
    "Counter",
    3,
    &bounce_counter,
    bounce_counter_stack,
    sizeof(bounce_counter_stack));

static GpioChangeDetector change_detector(
    TEST_PUSH_BUTTON_PIN,
    GpioChangeType::ANY_CHANGE,
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
      "GPIO change detector button bounce sketch compiled on %s at %s.\n",
      __DATE__,
      __TIME__);

  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(TEST_SIGNAL_OUT_PIN, OUTPUT);
  pinMode(TEST_SIGNAL_IN_PIN, INPUT);
  pinMode(TEST_PUSH_BUTTON_PIN, INPUT_PULLUP);

  if (!GpioChangeService.begin()) {
    halt_and_catch_fire("Could not start the GPIO change service.");
  }

  if (!bounce_counter.begin()) {
    halt_and_catch_fire("Could not start the bounce counter.");
  }

  bounce_counter_task.start();
  change_detector.start();

  Serial.println("Setup completed.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
