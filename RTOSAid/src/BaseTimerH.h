/*
 * BaseTimerH.h
 *
 *  Created on: Oct 16, 2025
 *      Author: Eric Mintz
 *
 * Base class for free running and one shot timer implementations.
 */

#ifndef SRC_BASETIMERH_H_
#define SRC_BASETIMERH_H_

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "VoidFunction.h"

/**
 * A course grained timer based on the FreeRTOS timer API that provides
 * the capabilities required for continuous (i.e. free running) and one shot
 * (i.e. stop on expiration) timers.
 *
 */
class BaseTimerH {
  const char *name_;
  VoidFunction& on_expiration_;
  const TickType_t default_timeout_;
  TimerHandle_t h_timer_;

protected:

  /**
   * Opens the timer. Users MUST invoke this method before invoking any other
   * non-constructor methods. Note that subclasses provide mechanisms to
   * do this.
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * auto_reload                 Free running flag: pdTRUE causes the timer
   *                             to rerun after expiration. pdFALSE causes it
   *                             to stop.
   * period                      Initial timeout period
   */
  bool begin(BaseType_t auto_reload, TickType_t period);

  /**
   * Returns: the default wait time for commands to be posted to the
   *          FreeRTOS timer queue
   */
  TickType_t default_timeout(void) {
    return default_timeout_;
  }

  /**
   * Returns: the queue handle, which will be NULL until begin() runs.
   */
  TimerHandle_t handle(void) {
    return h_timer_;
  }

  /**
   * Action to take when the timer expires. The implementation
   * invokes the VoidFunction that was bound at construction.
   */
  void on_timer_expired(void);

  /**
   * The timer callback. The underlying FreeRTOS timer invokes
   * this method when it expires.
   */
  static void timer_callback(TimerHandle_t timer_handle);

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
   */
  BaseTimerH(
      const char *name,
      VoidFunction& on_expiration,
      TickType_t default_timeout);


public:
  virtual ~BaseTimerH();

  /**
   * Stops the timer, waiting for the default timeout for the
   * command to be posted to the FreeRTOS timer queue. For use ONLY
   * by application code. Interrupt Service Routines (ISRs) MUST invoke
   * stop_from_isr().
   *
   * Returns: true if the stop request succeeded, false if it failed.
   */
  bool stop(void) {
    return stop(default_timeout_);
  }

  /**
   * Stops the timer, waiting for the specified timeout for the
   * command to be posted to the FreeRTOS timer queue. For use ONLY
   * by application code. Interrupt Service Routines (ISRs) MUST invoke
   * stop_from_isr().
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timeout                     The maximum number of ticks to wait for
   *                             the stop request to be enqueued on the
   *                             FreeRTOS timer command queue
   *
   * Returns: true if the stop request succeeded, false if it failed.
   */
  bool stop(TickType_t timeout) const {
    return xTimerStop(h_timer_, timeout);
  }

  /**
   * Stops the timer, returning immediately if the stop request cannot
   * be posted to the FreeRTOS timer queue. For use ONLY by Interrupt
   * Service Routines (ISRs). Application code MUST invoke stop().
   */
  bool stop_from_isr(void) const;

  /**
   * Check timer validity.
   *
   * Returns: true if the timer is valid, false otherwise. The timer
   *          is valid if and only if its handle is non-zero. Note that
   *          the method returns the value of NULL != h_timer_, NOT the
   *          handle value.
   */
  bool valid(void) const {
    return h_timer_;
  }
};

#endif /* SRC_BASETIMERH_H_ */
