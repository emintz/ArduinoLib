/**
 * StressMutex.ino
 *
 * Created on November 23, 2033
 *     Author: Eric Mintz
 *
 * A stress test of the Mutex class that attempts to create and detect
 * a race condition. The test passes if a race condition does not occur.
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
 * The test includes the following classes:
 *
 * 1. A heartbeat task that blinks the built-in LED. The blink indicates
 *    that the sketch is functioning normally.
 * 2. A target task that contains a critical code section. The critical
 *    code deliberately contains a race condition vulnerability.
 * 3. Two assailant tasks, run running at high priority, the other at low
 *    priority, that pound on the target task, attempting to cause a
 *    race condition.
 *
 * The assailant tasks illuminate LEDs when they hold the semaphore, so
 * it's easy to verify that they both run. They will also illuminate the
 * red led if they detect a race condition.
 *
 * Note that the task briefly illuminates all LEDS when it starts to
 * ensure that the LEDs are connected correctly.
 */

#include <StressTestBlinkAction.h>

#include "Arduino.h"
#include "AssailantAction.h"
#include "TargetClass.h"

#define RED_LED_PIN 13
#define YELLOW_LED_PIN 14
#define GREEN_LED_PIN 15
#define BLUE_LED_PIN 16

static uint8_t blink_stack[2048];
static StressTestBlinkAction blink_action;
static TaskWithAction blink_task(
    "Blink",
    1,
    &blink_action,
    blink_stack,
    sizeof(blink_stack));

static TargetClass target;

/**
 * The low priority assailant, which illuminates the yellow LED when it
 * holds the semaphore
 */
static uint8_t low_priority_stack[4096];
static AssailantAction low_priority_action(&target, 100, YELLOW_LED_PIN);
static TaskWithAction low_priority_task(
    "LowPriority",
    2,
    &low_priority_action,
    low_priority_stack,
    sizeof(low_priority_stack));

/**
 * The high priority task, which illuminates the green LED when it holds
 * the semaphore
 */
static uint8_t high_priority_stack[4096];
static AssailantAction high_priority_action(&target, 5, GREEN_LED_PIN);
TaskWithAction high_priority_task(
    "HighPriority",
    3,
    &high_priority_action,
    high_priority_stack,
    sizeof(high_priority_stack));

void setup() {
  Serial.begin(115200);
  Serial.printf(
      "Mutex stress test, built on %s at %s.\n",
      __DATE__,
      __TIME__);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, HIGH);

  vTaskDelay(pdMS_TO_TICKS(1000));

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  if (!target.begin()) {
    Serial.println("Mutex initialization failed.");
    for (;;) {
      vTaskDelay(portMAX_DELAY);
    }
  }

  blink_task.start();
  low_priority_task.start();
  high_priority_task.start();

  Serial.println("Setup completed.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
