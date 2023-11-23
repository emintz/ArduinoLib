/*
 * BasePullQueue.cpp
 *
 *  Created on: Nov 7, 2023
 *      Author: Eric Mintz
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
  return queue_handle = xQueueCreateStatic(
      queue_length,
      message_size,
      queue_storage,
      &queue_buffer);
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

bool BasePullQueue::really_send_message(void *message, TickType_t timeout) {
  return xQueueSendToBack(queue_handle, message, timeout) == pdTRUE;
}

bool BasePullQueue::really_send_message_from_ISR(void *message) {
  BaseType_t higher_priority_task_woken;
  bool result = xQueueSendToBackFromISR(
      queue_handle,
      message,
      &higher_priority_task_woken);
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
