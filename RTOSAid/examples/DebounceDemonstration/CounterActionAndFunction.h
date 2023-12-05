/*
 * GpioBounceCounter.h
 *
 *  Created on: Dec 3, 2023
 *      Author: Eric Mintz
 *
 * A VoidFunction that counts its invocations and a TaskWithAction that
 * displays the count and changes the yellow LED state every 15 seconds.
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

#ifndef COUNTERACTIONANDFUNCTION_H_
#define COUNTERACTIONANDFUNCTION_H_

#include "Arduino.h"

#include "MicrosecondTimer.h"
#include "Mutex.h"
#include "TaskAction.h"
#include "TaskWithAction.h"
#include "VoidFunction.h"

class CounterActionAndFunction :
    public TaskAction,
    public VoidFunction {

  Mutex mutex;

  volatile uint32_t count;

public:
  CounterActionAndFunction();
  ~CounterActionAndFunction();

  /**
   * Increment count, which tallys apply() invocations.
   */
  virtual void apply(void);

  /**
   * Initialize the class by initializing its mutex.
   */
  bool begin(void);

  /**
   * The task loop which toggles the yellow LED and prints count every 15
   * seconds.
   */
  virtual void run(void);
};

#endif /* COUNTERACTIONANDFUNCTION_H_ */
