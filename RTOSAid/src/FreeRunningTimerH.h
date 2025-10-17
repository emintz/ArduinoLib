/*
 * FreeRunningTimerH.h
 *
 *  Created on: Oct 16, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_FREERUNNINGTIMERH_H_
#define SRC_FREERUNNINGTIMERH_H_

#include "BaseTimerH.h"

class FreeRunningTimerH : public BaseTimerH {
  const TickType_t cycle_time_;

public:
  /**
   * Creates a free running timer having a specified name that invokes a
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
   * cycle_time                  Cycle time, the time between expirations
   * on_expiration               TimeExpiredAction this timer will invoke
   *                             upon expiration.
   * default_timeout             The default time (in ticks) to wait for
   *                             timer-related operations to complete.
   *                             Callers can override this value if desired.
   */
  FreeRunningTimerH(
      const char *name,
      VoidFunction& on_expiration,
      TickType_t cycle_time,
      TickType_t default_timeout = 0) :
        BaseTimerH(name, on_expiration, default_timeout),
        cycle_time_(cycle_time) {
  }

  virtual ~FreeRunningTimerH() = default;

  /**
   * Starts the timer. Applications MUST
   */
  bool begin(void);

  bool reset(void) {
    return reset(default_timeout());
  }

  bool reset(TickType_t timeout) {
    return xTimerReset(handle(), timeout);
  }

  bool start(void) {
    return start(default_timeout());
  }

  bool start(TickType_t timeout) {
    return xTimerStart(handle(), timeout);
  }
};

#endif /* SRC_FREERUNNINGTIMERH_H_ */
