/*
 * ActionInvokingOneShotTimer.cpp
 *
 *  Created on: Jul 10, 2023
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
 * Note that esp_timer_stop() returns ESP_ERR_INVALID_STATE if it is called
 * while the timer is not running. If we simply want to stop the timer and
 * don't care if it has already stopped, the error is harmless.
 */

#include "MicrosecondTimer.h"

MicrosecondTimer::~MicrosecondTimer() {
  target_function = NULL;
  if (h_timer) {
    esp_timer_stop(h_timer);  // ESP_ERR_INVALID_STATE is harmless; see above.
    esp_timer_delete(h_timer);
    h_timer = NULL;
  }
}

bool MicrosecondTimer::begin(void) {
  bool result = h_timer == NULL && mutex.begin();
  if (result) {
    Serial.println("Creating the timer.");
    esp_timer_create_args_t timer_create_args;
    memset(&timer_create_args, 0, sizeof(timer_create_args));
    timer_create_args.callback = timer_callback;
    timer_create_args.arg = this;
    timer_create_args.dispatch_method = ESP_TIMER_TASK;
    timer_create_args.name = timer_name;
    timer_create_args.skip_unhandled_events = true;
    esp_err_t create_result = esp_timer_create(&timer_create_args, &h_timer);
    Serial.printf("Timer create returned %d.\n", create_result);
    result = (create_result == ESP_OK);
  }
  return result;
}

bool MicrosecondTimer::start(uint64_t timeout_in_microseconds) {
  MutexLock lock(mutex);
  bool result = h_timer != NULL;
  if (result) {
    stop_if_running();
    result = (esp_timer_start_once(h_timer, timeout_in_microseconds) == ESP_OK);
    if (result) {
      timer_state = TimerState::RUNNING;
    }
  }
  return result;
}

void MicrosecondTimer::stop(void) {
  MutexLock lock(mutex);
  stop_if_running();
}

void MicrosecondTimer::stop_if_running() {
  if (timer_state == RUNNING) {
    esp_timer_stop(h_timer);
    timer_state = STOPPED;
  }
}

void MicrosecondTimer::timer_expired(void) {
  target_function->apply();
}

void MicrosecondTimer::timer_callback(void *params) {
  static_cast<MicrosecondTimer *>(params)->timer_expired();
}
