/*
 * BasePullQueueH.h
 *
 *  Created on: May 15, 2025
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

#ifndef SRC_BASEPULLQUEUEH_H_
#define SRC_BASEPULLQUEUEH_H_

#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class BasePullQueueH {
  const size_t message_size;
  const UBaseType_t queue_length;
  QueueHandle_t queue_handle;
protected:

  /*
   * Creates and configures a PullQueue instance whose storage
   * is allocated on the heap.
   *
   * Parameters
   *
   * Name          Contents
   * ------------- ----------------------------------------------------------
   * message_size  Length of the message in bytes, which must be > 0. Every
   *               enqueued enqueued message must have this exact length.
   * queue_length  Maximum number of messages that the queue can hold
   */
  BasePullQueueH(
      size_t message_size,
      UBaseType_t queue_length);

  virtual ~BasePullQueueH();

  /*
   * Peek a message from the queue. Returns the first message found in
   * the queue, if any, without removing it from the queue.
   *
   *
   * Parameters
   *
   * Name          Contents
   * ------------- ----------------------------------------------------------
   * message       Receives the peaked message -- the first message on the
   *               queue, if any. Contents are unspecified if the queue
   *               is empty.
   * timeout       The wait limit in ticks.
   *
   * Returns: true if a message was found within the specified timeout,
   * false otherwise.
   */
  bool really_peek_message(void *message, TickType_t timeout = portMAX_DELAY);


  /*
   * Pull a message from the queue. Returns the first message found in
   * the queue, if any, removing it from the queue.
   *
   *
   * Parameters
   *
   * Name          Contents
   * ------------- ----------------------------------------------------------
   * message       Receives the peaked message -- the first message on the
   *               queue, if any. Contents are unspecified if the queue
   *               is empty.
   * timeout       The wait limit in ticks.
   *
   * Returns: true if a message was found within the specified timeout,
   * false otherwise.
   */
  bool really_pull_message(void *message, TickType_t timeout = portMAX_DELAY);


  /*
   * Add a a message to the end of the queue
   *
   *
   * Parameters
   *
   * Name          Contents
   * ------------- ----------------------------------------------------------
   * message       The message to send
   * timeout       The wait limit in ticks.
   *
   * Returns: true if a message was sent within the specified timeout,
   * false otherwise. Send will fail if the queue is full.
   */
  bool really_send_message(
      const void * const message, TickType_t timeout = portMAX_DELAY);

  /*
   * Sends a message from an interrupt service routine (ISR) and yields if a
   * higher priority task is awakened.
   *
   * Returns true if and only if the send succeeded.
   */
  bool really_send_message_from_ISR(const void * const message);

public:

  /*
   * Returns the number available message storage, the number of messages
   * that can be enqueued before the queue becomes full. Be sure that the
   * queue is valid before you invoke this method. Invoking it on an invalid
   * queue will cause unspecified and almost certainly undesirable behavior.
   */
  UBaseType_t available_message_storage(void) const;

  /*
   * Start the queue and make it available to transport messages.
   *
   * Returns: true if the queue started successfully, false if the queue
   * could not be started.
   */
  bool begin(void);

  /*
   * Returns true if the queue is valid and can be used or false otherwise.
   * Note that this method will return false if begin() has not been invoked,
   * and that the user is responsible for guarding against race conditions.
   */
  bool valid(void) const;

  /*
   * Returns the number of waiting messages if the queue is valid. Calling this
   * method when the queue is not valid will cause undefined and almost
   * certainly undesirable behavior.
   */
  UBaseType_t waiting_message_count(void) const;

};

#endif /* SRC_BASEPULLQUEUEH_H_ */
