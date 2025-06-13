/*
 * ActionInvokingOneShotTimer.h
 *
 *  Created on: Jul 10, 2023
 *      Author: Eric Mintz
 *
 * A microsecond resolution one shot timer that invokes an action when it
 * fires. The action is set at construction. Note that this class is
 * multitasking-safe and thread-safe.
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

#ifndef MICROSECONDTIMER_H_
#define MICROSECONDTIMER_H_

#include "Arduino.h"
#include "Mutex.h"
#include "esp_timer.h"
#include "VoidFunction.h"

class TimerAction;

class MicrosecondTimer final {
public:
  enum TimerState {
    RUNNING,
    STOPPED,
  };
private:
  const char *timer_name;
  VoidFunction *target_function;
  esp_timer_handle_t h_timer;
  TimerState timer_state;
  Mutex mutex;

  /**
   * Stops the timer if it is running, and does nothing otherwise. To avoid
   * race conditions, callers must hold the mutex before they invoke this
   * method.
   */
  void stop_if_running(void);

  /**
   * Invoked when the timer fires. User is responsible for implementing
   * multi-task and thread safety if required.
   */
  static void timer_callback(void *params);

  /**
   * Invokes the bound Action. Note that the timer callback
   * invokes this.
   */
  void timer_expired(void);

public:
  /**
   * Constructor, creates a new MicrosecondTimer instance. Users must invoke
   * begin() before using the timer.
   *
   * Parameters
   *
   * Name            Contents
   * -----------     -----------------------------------------------------
   * name            Timer name, a NULL-terminated string containing the
   *                 timer's name for display in debug and error messages.
   * target_function A VoidFunction subclass containing the code to run when
   *                 the timer fires. Note: the function is invoked from an
   *                 ISR. Keep it short and do NOT block.
   */
  MicrosecondTimer(
      const char *timer_name,
      VoidFunction *target_function) :
        timer_name(timer_name),
        target_function(target_function),
        h_timer(NULL),
        timer_state(STOPPED),
        mutex() {
  }

  virtual ~MicrosecondTimer();

  /**
   * Initializes the timer and makes it usable. Invoke this exactly once before
   * using the timer.
   */
  bool begin(void);

  /**
   * Tears down the timer. When this function succeeds, the timer becomes
   * unusable, but can be started again. Make sure that the begin() has run
   * successfully before invoking this function.
   */
  bool end(void);

  /**
   * Starts the timer. If the timer is already running, its timeout is changed
   * to the specified value. Note that the Timer invokes task->notify() when
   * on expiration.
   */

  bool start(uint64_t timeout_in_microseconds);

  /**
   * Stops the timer.
   */
  void stop(void);

  TimerState state() {
    return timer_state;
  }
};

#endif /* MICROSECONDTIMER_H_ */
