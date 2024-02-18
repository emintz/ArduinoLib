/*
 * TaskWithActionH.cpp
 *
 *  Created on: Feb 5, 2024
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

#include "TaskWithActionH.h"

TaskWithActionH::TaskWithActionH(
    const char *name,
    const UBaseType_t priority,
    TaskAction *action,
    const size_t stack_size) :
      BaseTaskWithAction(
          action,
          name,
          priority,
          stack_size) {
}

TaskWithActionH::~TaskWithActionH() {
}

bool TaskWithActionH::start(void) {
  TaskHandle_t task_handle = NULL;
  BaseType_t create_status = xTaskCreate(
      run_task_loop,
      task_name(),
      task_stack_size(),
      this,
      task_priority(),
      &task_handle);
  set_task_handle(task_handle);
  return create_status == pdPASS;
}
