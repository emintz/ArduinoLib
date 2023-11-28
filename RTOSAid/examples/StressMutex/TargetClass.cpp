/*
 * TargetClass.cpp
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

#include "TargetClass.h"

TargetClass::TargetClass() {
  count = 0;
}

TargetClass::~TargetClass() {
}

bool TargetClass::begin(void) {
  return mutex.begin();
}

void TargetClass::have_a_go(uint32_t delay, RaceConditionStruct *result) {
  MutexLock lock(mutex);
  if (lock.succeeded()) {
    // Lock succeeded. Try to create a race condition. We hope the attempt
    // fails.
    result->entry_count = count;

    // Give other tasks a chance to invoke have_a_go(). If another
    // task somehow makes it past its semaphore lock, it will increment
    // the count, forcing this task's returned exit count to be greater
    // than its entry count + 1.
    vTaskDelay(pdMS_TO_TICKS(delay));  // Opportunity for preemption

    result->exit_count = ++count;
  } else {
    // Semaphore not locked. Force a failure.
    result->entry_count = 0;
    result->exit_count = 0;
  }
}
