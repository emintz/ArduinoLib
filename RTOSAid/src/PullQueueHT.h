/*
 * PullQueueHT.h
 *
 *  Created on: May 15, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_PULLQUEUEHT_H_
#define SRC_PULLQUEUEHT_H_

#include "BasePullQueueH.h"

template <class T> class PullQueueHT : public BasePullQueueH {
public:
  inline PullQueueHT(
      UBaseType_t queue_length) :
          BasePullQueueH(
              sizeof(T),
              queue_length) {
  }

  virtual ~PullQueueHT() {}

  /**
   * Attempt to retrieve a message from the queue leaving the retrieved message
   * in the queue if found. Wait the maximum allowable time for a message to
   * appear.
   *
   * Parameters:
   *
   * Name     Contents
   * -------- ----------------------------------------------------------------
   * message  The retrieved message, if found. Contents are not specified if
   *          a message was not pulled.
   *
   * Returns: true if a message appeared in the queue, false otherwise
   */
  inline bool peek_message(T *message) {
    return really_peek_message(message);
  }

  /**
   * Attempt to retrieve a message from the queue leaving the retrieved message
   * from the queue. Wait the specified time for a message to appear.
   *
   * Parameters:
   *
   * Name        Contents
   * ----------  --------------------------------------------------------------
   * message     The retrieved message, if found. Contents are not specified if
   *             a message was not pulled.
   * max_wait_ms The maximum number of milliseconds to wait for the queue
   *             to accept the message. If the wait time is zero, the call
   *             returns immediately.
   *
   * Returns: true if a message appeared in the queue, false otherwise
   */
  inline bool peek_message(T *message, uint32_t max_wait_ms) {
    return really_peek_message(message, pdMS_TO_TICKS(max_wait_ms));
  }

  /**
   * Attempt to retrieve a message from the queue removing the retrieved message
   * in the queue if found. Wait the maximum allowable time for a message to
   * appear.
   *
   * Parameters:
   *
   * Name     Contents
   * -------- ----------------------------------------------------------------
   * message  The retrieved message, if found. Contents are not specified if
   *          a message was not pulled.
   *
   * Returns: true if a message appeared in the queue, false otherwise
   */
  inline bool pull_message(T *message) {
    return really_pull_message(message);
  }

  /**
   * Attempt to retrieve a message from the queue removing the retrieved message
   * from the queue. Wait the specified time for a message to appear.
   *
   * Parameters:
   *
   * Name        Contents
   * ----------  --------------------------------------------------------------
   * message     The retrieved message, if found. Contents are not specified if
   *             a message was not pulled.
   * max_wait_ms The maximum number of milliseconds to wait for the queue
   *             to accept the message. If the wait time is zero, the call
   *             returns immediately.
   *
   * Returns: true if a message appeared in the queue, false otherwise
   */
  inline bool pull_message(T *message, uint32_t max_wait_ms) {
    return really_pull_message(message, pdMS_TO_TICKS(max_wait_ms));
  }

  /**
   * Add a message to the queue tail, waiting the maximum permitted time for
   * space to become available in the queue. If the call succeeds, the
   * subscriber will receive the newly sent message after it receives
   * previously enqueued messages, if any.
   *
   * Parameters:
   *
   * Name        Contents
   * ----------  --------------------------------------------------------------
   * message     The message to send.
   * max_wait_ms The maximum number of milliseconds to wait for the queue
   *             to accept the message. If the wait time is zero, the call
   *             returns immediately.
   *
   * Returns: true if the send succeeds, false otherwise. The call will fail
   * if the queue remains full throughout the wait period.
   */
  inline bool send_message(const T *const message) {
    return really_send_message(message);
  }


  /**
   * Add a message to the queue tail, waiting the specified milliseconds for
   * space to become available in the queue. If the call succeeds, the
   * subscriber will receive the newly sent message after it receives
   * previously enqueued messages, if any.
   *
   * Parameters:
   *
   * Name        Contents
   * ----------  --------------------------------------------------------------
   * message     The message to send.
   *
   * Returns: true if the send succeeds, false otherwise. The call will fail
   * if the queue remains full throughout the wait period.
   */
  inline bool send_message(const T * const message, uint32_t max_wait_ms) {
    return base_send_message(message, pdMS_TO_TICKS(max_wait_ms));
  }

  /**
   * Sends a message from an interrupt service routine (ISR) and yields if a
   * higher priority task is awakened. If the send succeeds, the message is
   * added to the queue's tail.
   * Parameters:
   *
   * Name        Contents
   * ----------  --------------------------------------------------------------
   * message     The message to send.
   * max_wait_ms The maximum number of milliseconds to wait for the queue
   *             to accept the message. If the wait time is zero, the call
   *             returns immediately.
   *
   * Returns: true if the send succeeds, false otherwise. The call will fail
   * if the queue remains full throughout the wait period.
   */
  inline bool send_message_from_ISR(T *message) {
    return base_send_message_from_ISR(message);
  }
};



#endif /* SRC_PULLQUEUEHT_H_ */
