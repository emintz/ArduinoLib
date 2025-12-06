/*
 * OneShotTimer.h
 *
 *  Created on: Oct 12, 2025
 *      Author: Eric Mintz
 *
 * One shot (single cycle) timer with action. This class uses the
 * FreeRTOS API and has a one FreeRTOS tick resolution. Use the
 * MicrosecondTimer for higher resolution.
 *
 * Copyright (C) 2025 Eric Mintz
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

#ifndef SRC_ONESHOTTIMERH_H_
#define SRC_ONESHOTTIMERH_H_

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "VoidFunction.h"

/**
 * A one shot timer, a timer that runs once then stops. The construction
 * binds the timer to a TimeExpiredAction that it invokes when it expires.
 * A running timer can be stopped before it expires, in which case it
 * does NOT invoke its action.
 *
 * To implement a watchdog timer, start a one-shot timer and reset it
 * by invoking start_XXX. The timer will fire if the time between
 *
 * start_XXX invocatations exceeds the timeout.
 *
 * Note: Many public FreeRTOS timer API functions send commands to the
 *       timer service via the timer command queue, a private queue
 *       that application code cannot access. Like all queues, it has
 *       finite capacity so timer functions could wait indefinitely
 *       for queue space to become available. Users can specify a
 *       maximum waiting time. Since the underlying timer is based
 *       on queues, methods in this class are inherently thread-safe.
 *
 *       Be aware of race conditions, especially when delays can be
 *       zero. Consider a OneShotTimerH bound to a NotifyingTimerAction.
 *       The bound task will use the timer as follows:
 *
 *           one_shot_timer.start_ms(timeout_in_ms);
 *           wait_for_notification();
 *
 *       If timout_in_ms is 0, the start_ms() call could return before
 *       the invoking task can invoke wait_for_notification causing the
 *       task to wait for the maximum possible delay.
 */
class OneShotTimerH {
  const char *name_;
  VoidFunction& on_expiration_;
  const TickType_t default_timeout_;
  TimerHandle_t h_timer;

  /**
   * The timer callback. The underlying FreeRTOS timer invokes
   * this method when it expires.
   */
  static void timer_callback(TimerHandle_t timer_handle);

  /**
   * The time expired callback function invokes this method.
   */
  void on_timer_expired(void);

public:
  /**
   * Creates a timer having a specified name that invokes a
   * specified expiration action.
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * name                        NULL-terminated timer name of this timer. The
   *                             name SHOULD be distinct among all timers in
   *                             the invoking application and SHOULD NOT be
   *                             empty or NULL. After initialization, the
   *                             underlying FREERTOS timer will bear this
   *                             name.
   * on_expiration               TimeExpiredAction this timer will invoke
   *                             upon expiration.
   * default_timeout             The default time (in ticks) to wait for
   *                             timer-related operations to complete.
   *                             Callers can override this value if desired.
   *                             Defaults to 0.
   */
  OneShotTimerH(
      const char *name,
      VoidFunction& on_expiration,
      TickType_t default_timeout = 0);
  virtual ~OneShotTimerH();

  /**
   * Query this time to see if it is active. Behavior is undefined until
   * begin() runs successfully.
   *
   * Returns: true if the timer is active (i.e. running); false otherwise.
   */
  bool active(void) {
    return xTimerIsTimerActive(h_timer);
  }

  /**
   * Initializes this timer. The timer becomes usable if this method succeeds.
   * Users must invoke this method EXACTLY ONCE before using the timer.
   * Invoking this method multiple times or attempting to use the timer
   * if this method has not been invoked will cause unspecified and
   * undesirable behavior.
   *
   * Returns: true on success (i.e. if this timer becomes usable), false
   *          on failure.
   */
  bool begin(void);

  /**
   * Returns: this timer's name
   */
  const char *name(void) {
    return name_;
  }

  /**
   * Start the timer with the specified countdown time in milliseconds.
   * Wait the specified time for the timer queue to accept the request.
   * Invoke this method ONLY from application code. Interrupt service
   * routines MUST invoke start_ticks_from_isr() instead;
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timeout_ms                  The expiration time in milliseconds
   * wait_time                   Number of ticks to wait for the timer queue
   *                             to accept the request.
   *
   * Return: true on success, false on failure.
   */
  bool start_ms(
      uint32_t timeout_ms, TickType_t wait_time) {
    return start_ticks(pdMS_TO_TICKS(timeout_ms), wait_time);
  }

  /**
   * Start the timer with the specified timeout in ticks and start it. Wait
   * the default time for the timer command queue to accept the request.
   * Interrupt service routines MUST invoke start_ticks_from_isr() instead;
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timeout_ticks               The expiration time in ticks
   *
   * Return: true on success, false on failure.
   */
  bool start_ms(uint32_t timeout_ms) {
    return start_ticks(pdMS_TO_TICKS(timeout_ms), default_timeout_);
  }

  /**
   * Change the timer period (i.e. timeout) to the specified timeout in ticks
   * and start it. Wait the specified time for the timer to accept the request.
   * Interrupt service routines MUST invoke start_ticks_from_isr() instead;
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timeout_ticks               The expiration time in ticks
   * wait_time                   Number of ticks to wait for the timer queue
   *                             to accept the request.
   *
   * Return: true on success, false on failure.
   */
  bool start_ticks(
      TickType_t timeout_ticks, TickType_t wait_time);


  /**
   * Start the timer with the specified timeout in ticks. Wait the default
   * time for the timer command queue to accept the request. Interrupt service
   * routines MUST invoke start_ticks_from_isr() instead;
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timeout_ticks               The expiration time in ticks
   *
   * Return: true on success, false on failure.
   */
  bool start_ticks(TickType_t timeout_ticks) {
    return start_ticks(timeout_ticks, default_timeout_);
  }

  /**
   * Start the timer with the specified timeout in ticks. Since interrupt
   * service routines cannot block, the call fails if the request cannot
   * be posted.
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timeout_ticks               The expiration time in ticks
   *
   * Return: true on success, false on failure.
   */
  bool start_ticks_from_isr(TickType_t timout_ticks);

  /**
   * Stop this timer if it is running. The ESP32 API documentation states that
   * stopping a timer ensures that it is not in the active [i.e. running] state.
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * wait_time                   Number of ticks to wait for the timer queue
   *                             to accept the request.
   *
   * Return: true on success, false on failure.
   */
  bool stop(TickType_t wait_time) {
    return xTimerStop(h_timer, wait_time);
  }

  /**
   * Stop this timer, waiting the default time for the timer to accept
   * the request. The ESP32 API documentation states that
   * stopping a timer ensures that it is not in the active [i.e. running] state.
   *
   * Return: true on success, false on failure.
   */
  bool stop(void) {
    return stop(default_timeout_);
  }

  bool stop_from_isr(void);

  /**
   * Queries this timers validity. The timer is usable if and only if
   * this method returns true.
   *
   * Returns: true if this timer is up and running, false otherwise.
   */
  bool valid(void) const {
    return NULL != h_timer;
  }
};

#endif /* SRC_ONESHOTTIMERH_H_ */
