/*
 * TaskWithAction.h
 *
 *  Created on: May 9, 2023
 *      Author: Eric Mintz
 *
 * Utility class that simplifies the creation and management of Free RTOS
 * tasks in ESP32-targeted applications
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
 * The TaslWotjActopm class implements a basic FreeRTOS task whose storage
 * is statically allocated and provided at construction. The task's logic,
 * a.k.a. its task loop, is implemented in an Action which is bound to the task
 * at construction. The user must start the task explicitly, and the running
 * task is not pinned to a core.
 *
 * Users must provide the following:
 *
 * 1. Task name, priority, and stack storage
 * 2. An action containing the task loop
 * 3. Stack storage
 *
 * The action must provide a task loop that should run indefinitely. The user
 * must completely configure the action before attempting to start it.
 *
 * FreeRTOS task loops must either run forever or terminating by terminating
 * their containing task. The TaskWithAction class complies with this
 * requirement by terminate itself if the action loop returns.
 *
 * Note that the Action class contains methods that correspond to some of
 * the methods defined below. Where possible, prefer the Action methods, as
 * we might add cleanup or error checking code to them in future releases.
 */

#ifndef TASKWITHACTION_H_
#define TASKWITHACTION_H_

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TaskAction;

class TaskWithAction {
  // Permits code in the TaskAction class access fields and functions in
  // this class.
  friend TaskAction;

  const char *name;
  TaskAction *action;
  UBaseType_t priority;
  StackType_t *stack;
  size_t stack_size;
  BaseType_t status;
  StaticTask_t task_buffer;
  TaskHandle_t task_handle;

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

protected:

  /**
   * Delay (pause) the task for the specified number of milliseconds. The task
   * loop will stop running for the specified time, and resume automatically
   * afterward.
   */
  void delay_millis(uint32_t millis);

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

  /**
   * Creates and configures a TaskWithAction instance. Note that the newly
   * created task will NOT be running. Invoke start() to run the task.
   *
   * Parameters
   *
   * Name           Contents
   * -------------- -------------------------------------------------------
   * name           Task name to display in error messages. Names must
   *                be unique and cannot be NULL.
   * priority       Task priority, with tasks having high valued priorities
   *                preempting tasks with low valued priorities. Priority
   *                values must be greater then 1 and less than
   *                configMAX_PRIORITIES, which is 25 at the time of this
   *                writing. Please see the documentation for details.
   * action         Contains the code for the task to run. Note that actions
   *                must publicly inherit Action.
   * stack          Stack storage for the task
   * stack_size     Number of bytes allocated for stack storage
   *
   * Lets we have an Action called action (see, e.g. PeriodicPulseAction). We
   * would wrap it in a TaskWithAction as follows:
   *
   * // Stack storage
   * static uint8_t action_task_stack[4096];
   *
   *
   * static TaskWithAction task(
   *     "ExampleTask",
   *     16,
   *     &action,
   *     action_stack,
   *     sizeof(action_stack));
   *
   */
  TaskWithAction(
      const char *name,
      uint16_t priority,
      TaskAction *action,
      void *stack,
      size_t stack_size);

  /**
   * Destructor -- runs when this TaskWithAction is deleted. As a defensive
   * action, the destructor deletes the associated FreeRTOS task if it is
   * still running. This is purely defensive and not guaranteed to work under
   * all circumstances. Applications should invoke stop() to end the
   * task.
   */
  virtual ~TaskWithAction();

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
  bool start(void);

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

#endif /* TASKWITHACTION_H_ */
