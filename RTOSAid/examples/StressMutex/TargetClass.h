/*
 * TargetClass.h
 *
 *  Created on: Nov 23, 2023
 *      Author: Eric Mintz
 *
 * Class containing a method that, in the absence of single threading,
 * would contain a race condition. Note that this class is not an action.
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
 * BE AWARE THAT THE CODE IN THIS EXAMPLE DELIBERATELY VIOLATES PROPER PRACTICE
 * The TargetClass's design contains deliberate flaws that maximize the
 * likelihood of flaky behavior.
 *
 * Critical code sections in production should run as quickly as possible, and
 * must never block. Blocking is only OK in tests that deliberately contain
 * race condition opportunities.
 *
 */

#ifndef TARGETCLASS_H_
#define TARGETCLASS_H_

#include "Arduino.h"

#include "Mutex.h"
#include "RaceConditionStruct.h"

class TargetClass {
  /**
   * We want every task to see the latest count, even if another task changed
   * it. Since optimization might hide another task's change, we disable
   * count-related optimization by declaring it volatile.
   */
  volatile uint32_t count;
  Mutex mutex;
public:
  TargetClass();
  virtual ~TargetClass();

  /**
   * Initializes the class, including its Mutex
   */
  bool begin(void);

  /**
   * A function containing a potential race condition. If its logic is not
   * serialized, i.e. limited to a single thread, it would exhibit
   * non-deterministic (a polite synonym for "flaky") behavior.
   *
   * The function maintains a count which it increments when invoked.
   *
   * Parameters:
   *
   * Name   Contents
   * ------ ------------------------------------------------------------------
   * delay  Time to hold the semaphore in milliseconds
   * result Receives the count at call entry and exit. When a race condition
   *        does not occur, the exit count equals the entry count + 1. Should
   *        a race condition occur, the exit count will be greater than the
   *        entry count + 1.
   */
  void have_a_go(uint32_t delay, RaceConditionStruct *result) ;
};

#endif /* TARGETCLASS_H_ */
