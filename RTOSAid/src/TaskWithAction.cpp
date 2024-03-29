/*
 * TaskWithAction.cpp
 *
 *  Created on: May 9, 2023
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
    BaseTaskWithAction(
        action,
        name,
        priority,
        stack_size),
  stack(static_cast<StackType_t *>(stack)) //,
{
  memset(&task_buffer, 0, sizeof(task_buffer));
}

TaskWithAction::~TaskWithAction() {
}

bool TaskWithAction::start(void) {
  return set_task_handle(xTaskCreateStatic(
      run_task_loop,
      task_name(),
      task_stack_size(),
      this,
      task_priority(),
      stack,
      &task_buffer));
}
