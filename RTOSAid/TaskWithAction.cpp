/*
 * TaskWithAction.cpp
 *
 *  Created on: May 9, 2023
 *      Author: Eric Mintz
 *
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
 *
 */

#include "TaskWithAction.h"

#include <stdlib.h>
#include "TaskAction.h"

TaskWithAction::TaskWithAction(
    const char *name,
    uint16_t priority,
    TaskAction *action,
    void *stack,
    size_t stack_size) :
  name(name),
  priority(priority),
  action(action),
  stack(static_cast<StackType_t *>(stack)),
  stack_size(stack_size),
  status(0),
  task_handle(NULL) {
  memset(&task_buffer, 0, sizeof(task_buffer));
  action->containing_task = this;
}

TaskWithAction::~TaskWithAction() {
  if (task_handle) {
    vTaskDelete(task_handle);
    task_handle = NULL;
  }
}

void TaskWithAction::run_task_loop(void *params) {
  static_cast<TaskWithAction *>(params)->start_task();
}

void TaskWithAction::delay_millis(uint32_t millis) {
  vTaskDelay(pdMS_TO_TICKS(millis));
}

void TaskWithAction::notify(void) {
  xTaskNotify(task_handle, 1, eSetValueWithoutOverwrite);
}

void IRAM_ATTR TaskWithAction::notify_from_isr(void) {
  BaseType_t higher_priority_task_woken;
  vTaskNotifyGiveFromISR(task_handle, &higher_priority_task_woken);
  if (higher_priority_task_woken) {
     portYIELD_FROM_ISR();
   }
}

bool TaskWithAction::start(void) {
  task_handle = xTaskCreateStatic(
      run_task_loop,
      name,
      stack_size,
      this,
      priority,
      stack,
      &task_buffer);

  return task_handle;
}

void TaskWithAction::resume(void) {
  vTaskResume(task_handle);
}

void TaskWithAction::start_task(void) {
  action->run();
  vTaskDelete(task_handle);
  task_handle = NULL;
}

void TaskWithAction::stop(void) {
  if (task_handle) {
    vTaskDelete(task_handle);
    task_handle = NULL;
  }
}

void TaskWithAction::suspend(void) {
  vTaskSuspend(task_handle);
}

uint32_t TaskWithAction::wait_for_notification(uint32_t millis) {
  return ulTaskNotifyTake(true, pdMS_TO_TICKS(millis));
}

void TaskWithAction::yield() {
  taskYIELD();
}
