/*
 * BasePullQueue.h
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
 *
 * A pull-oriented, multi-task and thread-safe FIFO (first in, first out)
 * inter-task communication queue. Application code pulls (i.e. reads)
 * messages from the queue. PullQueues have a fixed capacity and fixed
 * message length. Callers must provide queue storage at construction.
 *
 * The BasePullQueue is designed to be invoked by application code, though
 * interrupt service routines (ISR) can enqueue messages. This is a base
 * class for a templated, type-safe public implementation.
 */

#ifndef BASEPULLQUEUE_H_
#define BASEPULLQUEUE_H_

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class BasePullQueue {

  size_t message_size;
  UBaseType_t queue_length;
  uint8_t *queue_storage;
  QueueHandle_t queue_handle;
  StaticQueue_t queue_buffer;

protected:

  /**
   * Creates and configures a PullQueue instance
   *
   * Parameters
   *
   * Name          Contents
   * ------------- ----------------------------------------------------------
   * message_size  Length of the message in bytes, which must be > 0. Every
   *               enqueued enqueued message must have this exact length.
   * queue_length  Maximum number of messages that the queue can hold
   * queue_storage Storage for enqueued messages. This must have at least
   *               message_size * queue_length bytes.
   */
  BasePullQueue(
      size_t message_size,
      UBaseType_t queue_length,
      uint8_t *queue_storage);

  bool really_peek_message(void *message, TickType_t timeout = portMAX_DELAY);

  bool really_pull_message(void *message, TickType_t timeout = portMAX_DELAY);

  bool really_send_message(
      const void * const message, TickType_t timeout = portMAX_DELAY);

  /**
   * Sends a message from an interrupt service routine (ISR) and yields if a
   * higher priority task is awakened.
   *
   * Returns true if and only if the send succeeded.
   */
  bool really_send_message_from_ISR(const void * const message);

public:

  /**
   * Destructor, which runs when the queue is destroyed. If the underline
   * FreeRTOS queue is still running, the destrctor deletes it as a defensive
   * measure.
   */
  virtual ~BasePullQueue();

  /**
   * Returns the number available message storage, the number of messages
   * that can be enqueued before the queue becomes full. Be sure that the
   * queue is valid before you invoke this method. Invoking it on an invalid
   * queue will cause unspecified and almost certainly undesirable behavior.
   */
  UBaseType_t available_message_storage(void) const;

  /**
   * Start the queue and make it available to transport messages.
   *
   * Returns: true if the queue started successfully, false if the queue
   * could not be started.
   */
  bool begin(void);

  /**
   * Returns true if the queue is valid and can be used or false otherwise.
   * Note that this method will return false if begin() has not been invoked,
   * and that the user is responsible for guarding against race conditions.
   */
  bool valid(void) const;

  /**
   * Returns the number of waiting messages if the queue is valid. Calling this
   * method when the queue is not valid will cause undefined and almost
   * certainly undesirable behavior.
   */
  UBaseType_t waiting_message_count(void) const;
};

#endif /* BASEPULLQUEUE_H_ */
