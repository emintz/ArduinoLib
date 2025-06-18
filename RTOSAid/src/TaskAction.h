/*
 * Action.h
 *
 *  Created on: May 9, 2023
 *      Author: Eric Mintz
 *
 * Base class for providing logic, a.k.a the "task loop", that runs within
 * a TaskWithAction instance.
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
 * TaskAction's subclasses provide logic that a TaskWithAction runs.
 */

#ifndef TASKACTION_H_
#define TASKACTION_H_

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class BaseTaskWithAction;

class TaskAction {
  // Permits code in the BaseTaskWithAction class to access fields and
  // functions in this class
  friend BaseTaskWithAction;

  BaseTaskWithAction *containing_task;

protected:
  TaskAction(void);

  /**
   * Virtual destructor to ensure that the Action class can support dynamic
   * casting.
   */
  virtual ~TaskAction();

  /**
   * Delay (pause) the task for the specified number of milliseconds. The task
   * loop will stop running for the specified time, and resume automatically
   * afterward. The implementation forwards the request to the containing
   * TaskWithAction. Implementations should invoke it instead of accessing
   * the containing TaskWithAction directly.
   */
  virtual void delay_millis(uint32_t millis);

  inline bool task_status(void) {
    return containing_task != NULL;
  }

  /**
   * Resumes this task. Should be called when the task has been suspended.
   */
  virtual void resume(void);

  /**
   * Runs the task logic. Subclasses **must** override and implement this
   * function. The task logic should either loop indefinitely or call stop()
   * to exit.
   */
  virtual void run(void) = 0;

  /**
   * Stops the containing task. The current task is torn down and can no longer
   * be used. Prefer invoking calling stop() to returning from the task loop.
   * Note that this method does not return.
   */
  virtual void stop(void);

  /**
   * Suspend this task. The task logic is halted until the application calls
   * the containing task's or resume() function or this->resume().
   */
  virtual void suspend(void);

  /**
   * Suspend the task until some other task sends a notification or until
   * the specified delay elapses.
   *
   * Parameters
   *
   * Name            Contents
   * --------------- ----------------------------------------------------------
   * millis_to_wait  Delay in milliseconds, which defaults to the maximum
   *                 possible value if omitted. If millis_to_wait is zero,
   *                 the method will return immediately.
   *
   * Returns the task's notification count before the method was invoked.
   * Note that the method sets the notification count to 0, forcing the
   * task to wait. The implementation forwards the request to the containing
   * TaskWithAction. Implementations should favor it over accessing the
   * containing TaskWithAction directly.
   */
  virtual uint32_t wait_for_notification(uint32_t millis_to_wait=portMAX_DELAY);

  /**
   * Yield to any higher priority tasks that are ready to run. Note that
   * FreeRTOS implements cooperative multitasking, which means that it
   * cannot forcefully preempt a low priority task. Instead, low
   * priority tasks must volunteer for preemption. The implementation
   * forwards the request to the containing TaskWithAction. Implementations
   * should favor it over accessing the containing TaskWithAction directly.
   */
  void yield(void);
};


#endif // TASKACTION_H_
