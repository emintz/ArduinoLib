/*
 * MutexLock.h
 *
 *  Created on: Nov 23, 2023
 *      Author: Eric Mintz
 *
 * Mutual exclusion semaphore (mutex) lock.
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
 * Attempts to lock a Mutex, with the target Mutex specified at construction.
 * The user has the option of waiting forever or to specify a timeout
 * interval. To minimize the chance of deadlock, instances cannot be created
 * via new; they can only reside on the stack, meaning that they will be
 * destroyed when they go out of scope.
 *
 * To preserve anti-deadlock protection, all MutexLock instances must be
 * automatic variables, meaning that they **MUST NOT** be created via the
 * new operator or be declared as class fields. Applications **MUST NOT**
 * inherit from this class, which is why it's declared final.
 *
 * Note that importing Mutex.h also imports this file.
 */

#ifndef MUTEXLOCK_H_
#define MUTEXLOCK_H_

#include "Arduino.h"

class BaseMutex;

#include <new>

class MutexLock final {
  BaseMutex &mutex;
  bool lock_successful;

  /**
   * Copy construction makes no sense in the class, so we
   * hide same.
   */
  MutexLock(MutexLock *do_not_copy_me) = delete;
  MutexLock(MutexLock& do_not_copy_me) = delete;
  MutexLock& operator=(MutexLock const&) = delete;


  /**
   * Hiding the new and delete operators prevents allocation on the heap.
   * Locks must reside on the stack so that their constructors run
   * automatically when they go out of scope.
   */
  void* operator new  (std::size_t count) { return NULL; }
  void* operator new[](std::size_t count) { return NULL; }
  void* operator new  (
      std::size_t count, const std::nothrow_t& tag) { return NULL; }
  void* operator new[](
      std::size_t count, const std::nothrow_t& tag) { return NULL; }

  void operator delete  (void* ptr) {}
  void operator delete[](void* ptr) {}
  void operator delete  (void* ptr, const std::nothrow_t& tag) {}
  void operator delete[](void* ptr, const std::nothrow_t& tag) {}
  void operator delete  (void* ptr, std::size_t sz) {}
  void operator delete[](void* ptr, std::size_t sz) {}

public:
  /**
   * Creates a lock on a specified Mutex. The resulting instance holds the
   * lock until it is deleted. Since the lock attempt could, in principal,
   * fail, users should ensure that succeeded() returns true before
   * proceeding.
   *
   * Parameters:
   *
   * Name   Contents
   * ------ -----------------------------------------------------------------
   * mutex  The Mutex to lock
   */
  MutexLock(BaseMutex &mutex);

  /**
   * Locks the specified Mutex. If the Mutex is already locked, will wait
   * for the semaphore to become available. Since locking can
   * time out, users should ensure that succeeded() returns true before
   * proceeding.
   *
   * Parameters:
   *
   * Name   Contents
   * ------ -----------------------------------------------------------------
   * mutex  The Mutex to lock
   * wait_time_in_millis  The maximum number of milliseconds to wait to
   *                      lock the specified Mutex. If the lock cannot be
   *                      acquired before the specified deadline, the
   *                      lock attempt fails.
   */
  MutexLock(BaseMutex &mutex, uint32_t wait_time_in_millis);

  /**
   * Unlocks a semaphore. Does nothing if the lock fails.
   */
  ~MutexLock();

  /**
   * Returns true if and only if the underlying semaphore was locked
   * successfully.
   */
  inline bool succeeded() { return lock_successful; }
};

#endif /* MUTEXLOCK_H_ */
