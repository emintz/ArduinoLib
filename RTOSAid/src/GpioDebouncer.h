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

/**
 * The action for the debounce task, the task that receives the voltage
 * change notification
 */
class GpioDebouncerAction final : public TaskAction {
  friend class GpioDebouncer;
  MicrosecondTimer debounce_timer;
  uint32_t debounce_delay_micros;

  /**
   * Constructor, declared private to prevent application code from creating
   * instances
   *
   * Parameters:
   *
   * Name:                       Contents
   * --------------------------- ----------------------------------------------
   * timer_name                  Name of the class's MillisecondTimer
   * debounce_delay_microseconds Microseconds to wait before applying
   *                             function_to_call
   * function_to_call            VoidFunction to invoke after the switch
   *                             chatter settles
   */
  GpioDebouncerAction(
      const char * timer_name,
      uint64_t debounce_delay_micros,
      VoidFunction *function_to_call);

  virtual ~GpioDebouncerAction();
public:

  /**
   * Initializes the debouncer. When this method runs successfully,
   * debounce_timer is ready for use.
   *
   * Returns: true if initialization succeeded, false when it fails
   */
  bool begin(void);

  /**
   * Tears down debounce_timer
   */
  void end(void);

  /**
   * runs the task logic which starts debounce_timer to fire after
   * debounce_microseconds has elapsed.
   */
  virtual void run(void);
};

/**
 * VoidFunction that notifies a specified task when applied.
 */
class GoioDebounceFunction final : public VoidFunction {
  friend class GpioDebouncer;

  TaskWithAction *task;

  /**
   * Constructor
   *
   * Parameters:
   *
   * Name  Contents
   * ----- ----------------------------------------------
   * task  Task that apply() should notify
   */
  inline GoioDebounceFunction(TaskWithAction *task) :
      task(task) {
  }

  virtual ~GoioDebounceFunction();
public:

  virtual void apply(void);
};

/**
 * GPIO input pin voltage change detector that ignores switch chatter.
 */
class GpioDebouncer {
  GpioDebouncerAction action;
  uint8_t debounce_stack[2048];
  TaskWithAction debounce_task;
  GoioDebounceFunction on_pin_changed;
  GpioChangeDetector change_detector;

public:

  /**
   * Constructor
   *
   * Parameters:
   *
   * Name                  Contents
   * --------------------- ---------------------------------------------------
   * pin_no                GPIO pin to monitor
   * debounce_delay_micros Microseconds to wait before notifying the debounce
   *                       task that the pin voltage has changed
   * task_name             Debounce task name
   * priority              Debounce task priority
   * timer_name            Debounce timer name
   * function_to_call      Function to invoke on pin change
   */
  GpioDebouncer(
      uint8_t pin_no,
      uint32_t debounce_delay_micros,
      const char *task_name,
      uint16_t priority,
      const char *timer_name,
      VoidFunction *function_to_call);
  virtual ~GpioDebouncer();

  /**
   * Starts the detector
   */
  bool start(void);

  /**
   * Stops the detector
   */
  void stop(void);
};

#endif /* GPIODEBOUNCER_H_ */
