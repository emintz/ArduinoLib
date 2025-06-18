/*
 * BasePullQueue.cpp
 *
 *  Created on: Nov 7, 2023
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
 */

#include "Arduino.h"

#include "BasePullQueue.h"

#include <cstring>

BasePullQueue::BasePullQueue(
    size_t message_size,
    UBaseType_t queue_length,
    uint8_t *queue_storage) :
      message_size(message_size),
      queue_length(queue_length),
      queue_storage(queue_storage),
      queue_handle(NULL) {
  std::memset(&queue_buffer, 0, sizeof(queue_buffer));
}

BasePullQueue::~BasePullQueue() {
  if (queue_handle) {
    vQueueDelete(queue_handle);
    queue_handle = NULL;
  }
}

bool BasePullQueue::begin(void) {
  return (queue_handle = xQueueCreateStatic(
      queue_length,
      message_size,
      queue_storage,
      &queue_buffer));
}

UBaseType_t BasePullQueue::available_message_storage(void) const {
  return uxQueueSpacesAvailable(queue_handle);
}

bool BasePullQueue::really_peek_message(void *message, TickType_t timeout) {
  return xQueuePeek(queue_handle, message, timeout) == pdTRUE;
}

bool BasePullQueue::really_pull_message(void *message, TickType_t timeout) {
  return xQueueReceive(queue_handle, message, timeout) == pdTRUE;
}

bool BasePullQueue::really_send_message(
    const void * const message, TickType_t timeout) {
  return xQueueSendToBack(queue_handle, message, timeout) == pdTRUE;
}

bool BasePullQueue::really_send_message_from_ISR(const void * const message) {
  BaseType_t higher_priority_task_woken;
  bool result = (xQueueSendToBackFromISR(
      queue_handle,
      message,
      &higher_priority_task_woken));
  if (higher_priority_task_woken) {
    portYIELD_FROM_ISR();
  }
  return result;
}

bool BasePullQueue::valid(void) const {
  return queue_handle;
}

UBaseType_t BasePullQueue::waiting_message_count(void) const {
  return uxQueueMessagesWaiting(queue_handle);
}
