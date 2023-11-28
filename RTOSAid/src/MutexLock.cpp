/*
 * MutexLock.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: Eric Mintz
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

#include "MutexLock.h"

#include "Mutex.h"

MutexLock::MutexLock(
    Mutex &mutex) :
      mutex(mutex) {
  while (!(lock_successful = mutex.lock(portMAX_DELAY))) {}
}

MutexLock::MutexLock(
    Mutex &mutex,
    uint32_t wait_time_in_millis) :
      mutex(mutex) {
  lock_successful = mutex.lock(pdMS_TO_TICKS(wait_time_in_millis));
}

MutexLock::~MutexLock() {
  if (lock_successful) {
    mutex.unlock();
  }
}
