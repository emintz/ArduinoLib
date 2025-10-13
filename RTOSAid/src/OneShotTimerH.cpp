/*
 * OneShotTimer.cpp
 *
 *  Created on: Oct 12, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2025 Eric Mintz
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

#include "OneShotTimerH.h"

OneShotTimerH::OneShotTimerH(
    const char *name,
    VoidFunction& on_expiration,
    TickType_t default_timeout) :
      name_(name),
      on_expiration_(on_expiration),
      default_timeout_(default_timeout),
      h_timer(NULL) {
}

OneShotTimerH::~OneShotTimerH(void) {
  if (h_timer) {
    xTimerDelete(h_timer, default_timeout_);
  }
  h_timer = NULL;
}

void OneShotTimerH::timer_callback(TimerHandle_t timer_handle) {
  static_cast<OneShotTimerH *>(
      pvTimerGetTimerID(timer_handle)) -> on_timer_expired();
}

void OneShotTimerH::on_timer_expired(void) {
  on_expiration_.apply();
}

bool OneShotTimerH::begin(void) {
  /*
   * Callers provide a timeout when they start the timer
   * so the initial timeout is arbitrary.
   */
  h_timer = xTimerCreate(
      name_,
      portMAX_DELAY,
      pdFALSE,
      this,
      timer_callback);
  return valid();
}


bool OneShotTimerH::start_ticks(
    TickType_t timeout_ticks, TickType_t wait_time) {
  return xTimerChangePeriod(h_timer, timeout_ticks, wait_time);
}

bool OneShotTimerH::start_ticks_from_isr(TickType_t timeout_ticks) {
  BaseType_t higher_priority_task_woken = pdFAIL;
  bool result = xTimerChangePeriodFromISR(
      h_timer, timeout_ticks, &higher_priority_task_woken);
  if (higher_priority_task_woken) {
    portYIELD_FROM_ISR();
  }
}

bool OneShotTimerH::stop_from_isr(void) {
  BaseType_t higher_priority_task_woken = pdFAIL;
  bool result = xTimerStopFromISR(h_timer, &higher_priority_task_woken);
  if (higher_priority_task_woken) {
    portYIELD_FROM_ISR();
  }
  return result;
}
