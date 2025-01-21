/**
 * TaskWithAction-based sketch that blinks the built in LED once
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
 *
 * per second. This is the FreeRTOS analog of
 * https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/Blink/Blink.ino
 */
#include "Arduino.h"
#include "BlinkyAction.h"
#include "TaskWithAction.h"

#define BUILTIN_LED_PIN 2

// The blink action is extremely simple so it doesn't need much stack storage.

static uint8_t blink_action_stack[1024];
static BlinkyAction blink_action;
static TaskWithAction blink_task(
    "Blink",
    BUILTIN_LED_PIN,
    &blink_action,
    blink_action_stack,
    sizeof(blink_action_stack));

void setup() {
  Serial.begin(115200);
   Serial.printf(
       "Blinky, simple blink sketch, built on %s at %s.\n",
       __DATE__,
       __TIME__);

  blink_task.start();
  Serial.println("Blink started.");
}

void loop() {
  // There's nothing to do here, so let's minimize CPU load by waiting for
  // the longest allowable time.
  vTaskDelay(portMAX_DELAY);
}
