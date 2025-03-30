/*
 * BaseTaskWithAction.h
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

#ifndef LIBRARIES_RTOSAID_SRC_BASETASKWITHACTION_H_
#define LIBRARIES_RTOSAID_SRC_BASETASKWITHACTION_H_

#include "Arduino.h"
#include "TaskAction.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TaskAction;

class BaseTaskWithAction {
  friend TaskAction;

  TaskAction *action;
  const char *name;
  const UBaseType_t priority;
  const size_t stack_size;
  TaskHandle_t task_handle;
protected:
  BaseTaskWithAction(
      TaskAction *action,
      const char *name,
      const UBaseType_t priority,
      const size_t stack_size);

  inline const char *task_name(void) { return name; }
  inline const UBaseType_t task_priority(void) { return priority; }
  inline const size_t task_stack_size(void) { return stack_size; }
  bool set_task_handle(TaskHandle_t task_handle);

  /**
   * Delay (pause) the task for the specified number of milliseconds. The task
   * loop will stop running for the specified time, and resume automatically
   * afterward.
   */
  void delay_millis(uint32_t millis);

  /**
   * The task function. The parameters argument will contain a pointer to this
   * TaskWithAction.
   */
  static void run_task_loop(void *parameters);

  /**
   * Runs the task loop in the associated Action. Note that this TaskWithAction
   * will delete itself if the Action's run() method returns.
   */
  void start_task(void);

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
   * task to wait.
   */
  uint32_t wait_for_notification(uint32_t millis_to_wait=portMAX_DELAY);

  /**
   * Yield to any higher priority tasks that are ready to run. Note that
   * FreeRTOS implements cooperative multitasking, which means that it
   * cannot forcefully preempt a low priority task. Instead, low
   * priority tasks must volunteer for preemption.
   */
  void yield(void);

public:
  virtual ~BaseTaskWithAction();

  /**
   * Notify the task from application code. This will resume the task if it is
   * waiting for a notification. Does nothing if the task has a pending
   * notification.
   */
  void notify();

  /**
   * Notify the task from an interrupt service routine (ISR). Application code
   * must invoke notify() instead.
   */
  void IRAM_ATTR notify_from_isr();

  /**
   * Resumes a suspended task.
   */
  void resume(void);

  /**
   * Starts the task
   *
   * Returns the true if the task started and false if it failed to start.
   */
  virtual bool start(void) = 0;

  /**
   * Stop and delete the task, if it is running. This method is neither task
   * nor thread-safe.
   */
  void stop(void);

  /**
   * Suspends this task indefinitely. Invoke resume() to resume it. This
   * code is thread and multi-task safe. It is also idempotent, that is,
   * multiple invocations have the same effect as a single invocation.
   */
  void suspend(void);
};

#endif /* LIBRARIES_RTOSAID_SRC_BASETASKWITHACTION_H_ */
