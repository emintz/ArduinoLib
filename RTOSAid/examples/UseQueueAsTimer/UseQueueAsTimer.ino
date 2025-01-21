/**
 * UseQueueAsTimer.ino
 *
 *  Created on: Nov 19, 2023
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
 * PullQueueT<T>.peek_message() example that also shows how to use
 * TaskWithActionH, the heap-based version of TaskWithAction. The
 * sketch flashes the built-in LED irregularly.
 *
 * The sketch blinks the built-in LED rapidly in groups of five
 * flashes for 15 seconds, turns it off for 5 seconds, then
 * blinks it slowly in groups of seven for 10 seconds. The
 * sequence repeats indefinitely.
 */

#define BUILTIN_LED_PIN 2

#include "Arduino.h"

#include "BlinkStatusAction.h"
#include "PullQueueT.h"
#include "StatusBlinkCommand.h"
#include "TaskWithActionH.h"

#include <cstring>

static StatusBlinkCommand pull_queue_storage[3];
static PullQueueT<StatusBlinkCommand> blink_command_queue(
    pull_queue_storage,
    3);

static BlinkStatusAction action(BUILTIN_LED_PIN, &blink_command_queue);

static TaskWithActionH task(
    "Status",
    2,
    &action,
    4096);

static StatusBlinkCommand blink_messsage;

static StatusBlinkCommand fast_five = {
    100,
    100,
    500,
    5
};

static StatusBlinkCommand go_dark = {
    0,
    0,
    0,
    0
};

static StatusBlinkCommand slow_seven = {
    200,
    100,
    1000,
    7
};

void setup() {
   Serial.begin(115200);
   Serial.printf(
       "Queue use sketch compiled on %s at %s.\n",
       __DATE__,
       __TIME__);
   std::memset(&blink_messsage, 0, sizeof(blink_messsage));

   blink_command_queue.begin();
   task.start();

}

// The loop function is called in an endless loop
void loop() {
  blink_command_queue.send_message(&fast_five);
  vTaskDelay(pdMS_TO_TICKS(15000));
  blink_command_queue.send_message(&go_dark);
  vTaskDelay(5000);
  blink_command_queue.send_message(&slow_seven);
  vTaskDelay(pdMS_TO_TICKS(10000));
}
