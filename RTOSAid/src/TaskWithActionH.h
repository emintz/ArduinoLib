/*
 * TaskWithActionH.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Eric Mintz
 *
 * FreeRTOS Task with stack &c. allocated on the heap.
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

#ifndef LIBRARIES_RTOSAID_SRC_TASKWITHACTIONH_H_
#define LIBRARIES_RTOSAID_SRC_TASKWITHACTIONH_H_

#include "Arduino.h"
#include "BaseTaskWithAction.h"
#include "TaskAction.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TaskWithActionH : public BaseTaskWithAction {
public:
  TaskWithActionH(
      const char *name,
      const UBaseType_t priority,
      TaskAction *action,
      const size_t stack_size);
  virtual ~TaskWithActionH();

  /**
   * Starts the task
   *
   * Returns the true if the task started and false if it failed to start.
   */
  virtual bool start(void);

};

#endif /* LIBRARIES_RTOSAID_SRC_TASKWITHACTIONH_H_ */
