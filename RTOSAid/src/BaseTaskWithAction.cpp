/*
 * BaseTaskWithAction.cpp
 *
 *  Created on: Feb 4, 2024
 *      Author: Eric Mintz
 *
 * Copyright (C) 2024 Eric Mintz
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

#include "BaseTaskWithAction.h"

BaseTaskWithAction::BaseTaskWithAction(
    TaskAction *action,
    const char *name,
    const UBaseType_t priority,
    const size_t stack_size) :
      action(action),
      name(name),
      priority(priority),
      stack_size(stack_size),
      task_handle(NULL) {
  action->containing_task = this;
}

BaseTaskWithAction::~BaseTaskWithAction() {
  if (task_handle) {
    vTaskDelete(task_handle);
    task_handle = NULL;
  }
}

bool BaseTaskWithAction::set_task_handle(TaskHandle_t task_handle) {
  this->task_handle = task_handle;
  return this->task_handle;
}


void BaseTaskWithAction::delay_millis(uint32_t millis) {
  vTaskDelay(pdMS_TO_TICKS(millis));
}

void BaseTaskWithAction::notify(void) {
  xTaskNotify(task_handle, 1, eSetValueWithoutOverwrite);
}

void IRAM_ATTR BaseTaskWithAction::notify_from_isr(void) {
  BaseType_t higher_priority_task_woken;
  vTaskNotifyGiveFromISR(task_handle, &higher_priority_task_woken);
  if (higher_priority_task_woken) {
     portYIELD_FROM_ISR();
   }
}

void BaseTaskWithAction::resume(void) {
  vTaskResume(task_handle);
}

void BaseTaskWithAction::run_task_loop(void *params) {
  static_cast<BaseTaskWithAction *>(params)->start_task();
}

void BaseTaskWithAction::start_task(void) {
  action->run();
  vTaskDelete(task_handle);
  task_handle = NULL;
}

void BaseTaskWithAction::stop(void) {
  if (task_handle) {
    vTaskDelete(task_handle);
    task_handle = NULL;
  }
}

void BaseTaskWithAction::suspend(void) {
  vTaskSuspend(task_handle);
}

uint32_t BaseTaskWithAction::wait_for_notification(uint32_t millis) {
  return ulTaskNotifyTake(true, pdMS_TO_TICKS(millis));
}

void BaseTaskWithAction::yield() {
  taskYIELD();
}
