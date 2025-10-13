/*
 * BaseSemaphore.h
 *
 *  Created on: Feb 9, 2024
 *      Author: Eric Mintz
 *
 * The base class for static and heap-based mutual exclusion semaphores.
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

#ifndef LIBRARIES_RTOSAID_SRC_BASEMUTEX_H_
#define LIBRARIES_RTOSAID_SRC_BASEMUTEX_H_

#include "Arduino.h"

#include "MutexLock.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class BaseMutex {
  friend MutexLock;

  SemaphoreHandle_t semaphore_handle;

  /**
   * Locks the semaphore, returning true if the semaphore is locked and false
   * if locking failed. The method is idempotent; locking a locked
   * semaphore will succeed, but will have no effect. If an invocation should
   * fail, a subsequent invocation might succeed.
   *
   * The lock() method is reserved for the inner Lock class's exclusive use.
   * Accessed via the Lock constructor. This method is thread-safe.
   */
  bool lock(TickType_t wait_time_in_ticks);

  /**
   * Unlocks the semaphore. No matter how many times the semaphore has been
   * locked, the semaphore will be unlocked. This method is thread-safe.
   */
  void unlock(void);

protected:
  BaseMutex();
public:
  virtual ~BaseMutex();

protected:

  /**
   * Sets this instance's semaphore handle to the provided value. Returns
   * true if the provided value is non-NULL and false if it is null. This
   * method is not thread-safe.
   */
  inline bool set_handle(SemaphoreHandle_t handle) {
    return NULL != (semaphore_handle = handle);
  }

public:

  /**
   * Returns true if and only if this semaphore is valid, i.e. available for
   * locking. Note that valid will returns false until an invocation of
   * begin() succeeds.
   */
  inline bool valid(void) const {
    return semaphore_handle != NULL;
  }

  /**
   * Activate the mutex. Be sure to invoke this method before attempting to
   * lock the mutex. It is strongly recommended to invoke this method during
   * setup because it is not thread-safe.
   */
  virtual bool begin(void) = 0;
};

#endif /* LIBRARIES_RTOSAID_SRC_BASEMUTEX_H_ */
