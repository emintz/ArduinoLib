/**
 * OneShotTimer.ino
 *
 *  Created on: Oct 12, 2025
 *      Author: Eric Mintz
 *
 * OneShotTimer example of use.
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
#include "Arduino.h"

#include "BlinkLightAction.h"

#include "OneShotTimerH.h"
#include "TaskNotifierVoidFunction.h"
#include "TaskWithActionH.h"

static BlinkLightAction blink_action;
static TaskWithActionH blink_task(
    "Blink Task",
    10,
    &blink_action,
    4096);
TaskNotifierVoidFunction on_time_expired(blink_task);
OneShotTimerH blink_timer(
    "Blink Timer",
    on_time_expired);

void setup() {
  Serial.begin(115200);
  Serial.printf("OneShotTimer example sketch build on %s at %s.\n",
      __DATE__, __TIME__);
  blink_action.begin(blink_timer);
  Serial.println(
      blink_timer.begin()
      ? "Blink action started."
      : "Blink action failed to start.");
  Serial.println(
      blink_task.start()
      ? "Blink task started."
      : "Blink task failed to start.");
  Serial.println("Startup completed.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
