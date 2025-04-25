/*
 * CurrentTaskBlocker.h
 *
 *  Created on: Apr 20, 2025
 *      Author: Eric Mintz
 *
 * Blocks the current (i.e. invoking) task until notified. This is
 * meant to be used when the current task handle is not known. Prefer
 * the native notification API when running within a TaskWithAction or
 * TasksWithActionH.
 *
 * The layout of fields on a web page. Having this class saves memory by
 * allowing multiple web pages to use the same set of fields.
 *
 * Copyright (c) 2025, Eric Mintz
 * All Rights reserved.
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

#ifndef CURRENTTASKBLOCKER_H_
#define CURRENTTASKBLOCKER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * Provides a mechanism by which a task can wait for notification by
 * another task. This is a cheap and cheerful alternative to using
 * a semaphore.
 *
 * The supported use case requires two tasks: a receiver, which
 * waits to be notified, and the sender, which that provides the
 * notification. The receiving tasks owns (and must create) the
 * CurrentTaskBlocker instance, hence the class's name.
 *
 * Typical use:
 *
 * 1. The current task, the one that will be notified, creates
 *    a CurrentTaskBlocker instance.
 * 2. The receiving (i.e. current) task passes the sender a pointer
 *    or reference to the instance.
 * 3. The current task invokes CurrentTaskBlocker::wait(), which
 *    blocks it pending notification.
 * 4. The sender invokes CurrentTaskBlocker::notify().
 * 5. The resulting notification wakes the receiver causing its
 *    CurrentTaskBlocker::wait() invocation to return.
 *
 * Sometimes, depending on task priority, surrounding activity, or the
 * phase of the moon, the sender will invoke CurrentTaskBlocker::notify()
 * before the receiver invokes CurrentTaskBlocker::wait(), in which case
 * the wait() invocation returns immediately
 *
 * Note that the task that creates a CurrentTaskBlocker **MUST** be the
 * task that invokes CurrentTaskBlocker::wait(). If a different task invokes
 * it, it will never be notified and will wait indefinitely.
 *
 * Note that wait() times out after 4,294,967,295 microseconds, a little over
 * 49 days.
 */
class CurrentTaskBlocker final {
  TaskHandle_t h_invoking_task;

  /*
   * It doesn't matter which task invokes notify() or notify_from_isr(); the
   * waiting process will receive the same notification. This implies that
   * any number of tasks can invoke a single CurrentTaskBlocker instance
   * without conflict. Hence any number of classes can reference an instance.
   * Copying or assigning instance add no useful functionality and could
   * cause undesirable behavior, so we forbid both.
   */
  CurrentTaskBlocker(CurrentTaskBlocker *) = delete;
  CurrentTaskBlocker(const CurrentTaskBlocker *) = delete;
  CurrentTaskBlocker(CurrentTaskBlocker&) = delete;
  CurrentTaskBlocker(const CurrentTaskBlocker&) = delete;
  CurrentTaskBlocker(CurrentTaskBlocker&&) = delete;
  CurrentTaskBlocker(const CurrentTaskBlocker&&) = delete;
  CurrentTaskBlocker& operator=(CurrentTaskBlocker&) = delete;
  CurrentTaskBlocker& operator=(const CurrentTaskBlocker&) = delete;
  CurrentTaskBlocker& operator=(const CurrentTaskBlocker *) = delete;
public:
  /**
   * Constructor
   *
   * Creates an instance for use *only* by the invoking task.
   */
  CurrentTaskBlocker();

  /*
   * Destructor.
   *
   * Behavior is undefined (and almost certainly catastrophic) if
   * the sender should invoke a destroyed instance.
   */
  virtual ~CurrentTaskBlocker();

  /*
   * Notifies the task if a notification is not pending. The task is
   * notified even if it is not suspended, which will cause the next
   * wait() invocation to return immediately. This method is idempotent.
   *
   * Note that this method MUST only be invoked from application (i.e.
   * non-interrupt handling) code. Interrupt handlers MUST invoke
   * notify_from_isr instead. Violations will cause immediate and
   * catastrophic consequences.
   */
  void notify(void);

  /*
   * Notifies the task if a notification is not pending. The task is
   * notified even if it is not suspended, which will cause the next
   * wait() invocation to return immediately. This method is idempotent.
   *
   * Note that this method MUST only be invoked from interrupt handlers.
   * Application code MUST invoke notify() instead. Violations will
   * cause immediate and catastrophic consequences.
   */
  void IRAM_ATTR notify_from_isr(void);

  /*
   * Wait for notification or until the maximum possible delay expires.
   * Note that there is no way to distinguish between the two. Since the
   * longest possible delay is 4,294,967,295 microseconds, a little over
   * 49 days,, this should not present a problem.
   */
  void wait(void);
};

#endif /* CURRENTTASKBLOCKER_H_ */
