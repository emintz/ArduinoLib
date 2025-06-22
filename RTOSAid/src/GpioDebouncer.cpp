/*
 * GpioDebouncer.cpp
 *
 *  Created on: Dec 3, 2023
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

#include "GpioDebouncer.h"

GpioDebouncerAction::GpioDebouncerAction(
    const char * timer_name,
    uint64_t debounce_delay_micros,
    VoidFunction *function_to_call) :
      debounce_timer(timer_name, function_to_call),
      debounce_delay_micros(debounce_delay_micros) {
}

GpioDebouncerAction::~GpioDebouncerAction() {
}

bool GpioDebouncerAction::begin(void) {
  Serial.println("GpioDebouncerAction::begin called.");
  return debounce_timer.begin();
}

void GpioDebouncerAction::end(void) {
  debounce_timer.end();
}

void GpioDebouncerAction::run(void) {
  Serial.println("GpioDebouncerAction::run called.");
  for (;;) {
    wait_for_notification();
    debounce_timer.start(debounce_delay_micros);
  }
}

GoioDebounceFunction::~GoioDebounceFunction() {
}

void GoioDebounceFunction::apply(void) {
  task->notify_from_isr();
}

GpioDebouncer::GpioDebouncer(
    uint8_t pin_no,
    uint32_t debounce_delay_micros,
    const char *task_name,
    uint16_t priority,
    const char *timer_name,
    VoidFunction *function_to_call) :
      action(
          timer_name,
          debounce_delay_micros,
          function_to_call),
      debounce_task(
          task_name,
          priority,
          &action,
          debounce_stack,
          sizeof(debounce_stack)),
      on_pin_changed(&debounce_task),
      change_detector(
          pin_no,
          GpioChangeType::ANY_CHANGE,
          &on_pin_changed) {
}

GpioDebouncer::~GpioDebouncer() {
}

bool GpioDebouncer::start(void) {
  return
      action.begin()
      && change_detector.start()
      && debounce_task.start();
}

void GpioDebouncer::stop(void) {
  debounce_task.stop();
  action.end();
}
