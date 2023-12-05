/*
 * GpioDebouncer.h
 *
 *  Created on: Dec 3, 2023
 *      Author: Eric Mintz
 *
 * Detects GPIO changes with debounce delay.
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
 * GPIO voltage monitor that responds to any transition, LOW to HIGH or
 * HIGH to LOW after waiting a specified delay. The delay allows mechanical
 * switches to settle into a change.
 *
 * The contacts in a mechanical switch vibrate when they change so what
 * we consider a single change -- open to closed or closed to open --
 * actually generates a sequence of opens and closing that resembles the
 * following switch closure.
 *
 * Open   --> -------+  +---+      +--------+
 *                   |  |   |      |        |
 *                   |  |   |      |        |
 * Closed -->        +--+   +------+        +----------------------- ...
 *
 *                   |<---- Bounce Time --->|
 *
 * The solution is to out wait the bounce before responding to the change by
 * starting a timer on any signal change, making sure that the timer will
 * out wait the longest conceivable spurious transition. When the timer
 * eventually expires, we can be confident that the switch has settled
 * down.
 *
 * Since the debouncer must respond to all transitions, the triggered code
 * must read the pin to determine the switch state.
 *
 * The design requires that
 *
 * 1, Bounce is consistent over time.
 * 2. There is always enough time to read the switch state after it finishes
 *    bouncing.
 *
 * These assumptions hold for manually operated switches.
 *
 */

#ifndef GPIODEBOUNCER_H_
#define GPIODEBOUNCER_H_

#include "Arduino.h"

#include "GpioChangeDetector.h"
#include "MicrosecondTimer.h"
#include "TaskAction.h"
#include "TaskWithAction.h"
#include "VoidFunction.h"

class GpioDebouncerAction final : public TaskAction {
  friend class GpioDebouncer;
  MicrosecondTimer debounce_timer;
  uint32_t debounce_delay_micros;

  GpioDebouncerAction(
      char * timer_name,
      uint64_t debounce_delay_micros,
      VoidFunction *function_to_call);

  virtual ~GpioDebouncerAction();
public:

  bool begin(void);

  void end(void);

  virtual void run(void);
};

class GoioDebounceFunction final : public VoidFunction {
  friend class GpioDebouncer;

  TaskWithAction *task;

  inline GoioDebounceFunction(TaskWithAction *task) :
      task(task) {
  }

  virtual ~GoioDebounceFunction();
public:

  virtual void apply(void);
};


class GpioDebouncer {
  GpioDebouncerAction action;
  uint8_t debounce_stack[2048];
  TaskWithAction debounce_task;
  GoioDebounceFunction on_pin_changed;
  GpioChangeDetector change_detector;

public:
  GpioDebouncer(
      uint8_t pin_no,
      uint32_t debounce_delay_micros,
      char *task_name,
      uint16_t priority,
      char *timer_name,
      VoidFunction *function_to_call);
  virtual ~GpioDebouncer();

  bool start(void);

  void stop(void);
};

#endif /* GPIODEBOUNCER_H_ */
