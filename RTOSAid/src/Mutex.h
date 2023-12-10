/*
 * Mutex.h
 *
 *  Created on: May 10, 2023
 *      Author: Eric Mintz
 *
 * A mutual exclusion (a.k.a. mutex) semaphore with a lock that, if used
 * properly, is guaranteed to release the semaphore eventually.
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
 * Mutex semaphores (more briefly, "mutexes") are used to ensure that at
 * most one thread or task is running a block of code. The primary use
 * case is protecting code from race conditions.
 *
 * If a task or thread locks a mutex indefinitely, the system can freeze.
 * Using exceptions can significantly raise the risk of deadlock. The following
 * implementation minimizes this risk by partitioning the code into two
 * classes, the mutex proper, and a mutex lock. The lock provides the only
 * user-visible mechanism for acquiring and holding a mutex. Locks must only
 * be declared within methods. The implementation declares the class's
 * new operators to be private, so applications cannot allocate them on the
 * heap. Creating static instances, while possible, is absolutely
 * prohibited.
 *
 * The mutex lock and unlock methods are declared private, so application
 * code cannot invoke them. Since the Lock class is declared within the
 * Mutex class, it can access private fields and methods in the Mutex class,
 * and so is able to lock and unlock the semaphore.
 *
 * Here's a simple example of use:
 *
 * static Mutex mutex;
 *
 * void setup(void) {
 *   ...
 *   mutex.begin();  // Assuming that this call succeeds.
 *   ...
 * }
 *
 * void loop(void) {
 *   ...
 * }
 *
 * void critical_code(void) {
 *
 *   // Creating a Lock locks the semaphore. The desired wait time is omitted
 *   // here, so the constuctor will want forever to lock the Mutex.
 *   MutexLock lock(&mutex);
 *   if (lock.succeeded()) {
 *     // Critical code runs here.
 *   } else {
 *     // Handle the lock failure here.
 *   }
 *
 *   // Lock destructor, which releases the semaphore, runs here automatically.
 * }
 */

#ifndef LIBRARIES_ESP32_UTILITIES_MUTEX_H_
#define LIBRARIES_ESP32_UTILITIES_MUTEX_H_

#include "Arduino.h"

#include "MutexLock.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include <new>

class MutexLock;

class Mutex final {
  friend class MutexLock;

  StaticSemaphore_t mutex_buffer;
  SemaphoreHandle_t semaphore_handle;

  /**
   * Locks the semaphore, returning true if the semaphore is locked and false
   * if locking failed. The method is idempotent; locking a locked
   * semaphore will succeed, but will have no effect. If an invocation should
   * fail, a subsequent invocation might succeed.
   *
   * The lock() method is reserved for the inner Lock class's exclusive use.
   * Accessed via the Lock constructor.
   */
  bool lock(TickType_t wait_time_in_ticks);

  /**
   * Unlocks the semaphore. No matter how many times the semaphore has been
   * locked, the semaphore will be unlocked.
   */
  void unlock(void);

public:
  /**
   * Create an instance containing a FreeRTOS semaphore. Since FreeRTOS
   * semaphore initialization can fail, users should invoke valid() to
   * verify all is well.
   */
  Mutex(void);

  /**
   * Destructor. Cleans up when the mutex is no longer needed.
   */
  virtual ~Mutex();

  /**
   * Initialize the mutex and prepare it for locking. Returns true if
   * initialization succeeded and false if it failed.
   */
  bool begin();

  /**
   * Returns true if and only if this semaphore is valid, i.e. available for
   * locking. Note that valid will returns false until an invocation of
   * begin() succeeds.
   */
  inline bool valid(void) {
    return semaphore_handle != NULL;
  }
};

#endif /* LIBRARIES_ESP32_UTILITIES_MUTEX_H_ */
