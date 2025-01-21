/*
 * NotifyTaskPeriodically.h
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 *
 * Notifies a task at a specified interval. The task and interval are set
 * during construction.
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

#ifndef NOTIFYTASKPERIODICALLY_H_
#define NOTIFYTASKPERIODICALLY_H_

#include "Arduino.h"

#include "TaskAction.h"

class TaskWithAction;

/**
 * Task action that notifies a specified task at a specified
 * interval. The task and interval are set at construction
 * and remain constant throughout the instance lifetime.
 */
class NotifyTaskPeriodically : public TaskAction {
  TaskWithAction *task_to_notify;
  uint32_t notification_interval;
public:

  /**
   * Constructor that creates a new instance bound to the specified task and
   * notification interval
   *
   * Parameters:
   *
   * Name                   Contents
   * ---------------------  -------------------------------------------------
   * task_to_notify         Task to notify, Cannot be NULL
   * notification_interval  Notification interval in milliseconds. Must be
   *                        > 0.
   */
  NotifyTaskPeriodically(
      TaskWithAction *task_to_notify,
      uint32_t notification_interval);

  virtual ~NotifyTaskPeriodically();

  /**
   * The task loop that sends notifications to the target task.
   */
  virtual void run(void);
};

#endif /* NOTIFYTASKPERIODICALLY_H_ */
