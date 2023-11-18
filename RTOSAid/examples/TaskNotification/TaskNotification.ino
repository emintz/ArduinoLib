/**
 * Task notification example that runs two tasks, a notification receiver that
 * changes a GPIO level when notified, and a notification sender that notifies
 * the receiver periodically.
 *
 * Note that the task that invokes the loop() function runs at priority 1.
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

#include "Arduino.h"

#include "ChangeGPIOLevelOnNotification.h"
#include "NotifyTaskPeriodically.h"
#include "TaskWithAction.h"

#define BUILTIN_LED 2

#define NOTIFICATION_INTERVAL 500

/**
 * The notification receiving task, composed of a stack, an action that
 * changes a GPIO level, and a controlling task. The receiving task has the
 * highest priority in the application.
 */
uint8_t receiving_task_stack[4096];
ChangeGPIOLevelOnNotification receiving_action(BUILTIN_LED);
TaskWithAction receiving_task(
    "Receiver",
    3,
    &receiving_action,
    receiving_task_stack,
    sizeof(receiving_task_stack));

/**
 * The notification sending task, composed of a stack, an action that
 * notifies the receiving task, and a controlling task. This task runs
 * at a lower priority than the receiving task and at a higher priority
 * than the Arduino sketch.
 */
uint8_t sending_task_stack[4096];
NotifyTaskPeriodically notifying_action(&receiving_task, 500);
TaskWithAction sending_task(
    "Sender",
    2,
    &notifying_action,
    sending_task_stack,
    sizeof(receiving_task_stack));

void setup() {
  Serial.begin(115200);
  Serial.printf(
      "Task notify example built on %s at %s.\n",
      __DATE__,
      __TIME__);
  /**
   * Start the receiving task. Since it has a higher priority than its caller,
   * it will preempt the caller, that is, this task. This task will resume
   * when the receiving tasks starts to wait for its first notification. This
   * ensures that the receiver is up and running before the sender starts.
   */
  receiving_task.start();

  /**
   * Start the sender. Because the receiver is guaranteed to be running, this
   * is 100% safe. We must be careful.
   */
  sending_task.start();
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
