/*
 * CanPayload.h
 *
 *  Created on: May 26, 2025
 *      Author: Eric Mintz
 *
 * Message payload, it's identifier (i.e. type) and 0 - 8 byte
 * data.
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

#ifndef LIBRARIES_CANBUS_SRC_CANPAYLOAD_H_
#define LIBRARIES_CANBUS_SRC_CANPAYLOAD_H_

#include "Arduino.h"

#include "driver/twai.h"

class CanBus;
class CanPayloadAction;

class CanPayload final{

  friend CanBus;
  friend CanPayloadAction;

  twai_message_t twai_message;

  inline const twai_message_t& as_twai_message(void) const {
    return twai_message;
  }

  inline twai_message_t& as_twai_message(void) {
    return twai_message;
  }

public:
  /**
   * Constructor that creates an empty instance.
   */
  CanPayload();

  /*
   * Constructor that creates an instance containing the specified
   * message
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * twai_messasge     The incoming messsage to place in the newly
   *                   constructed instance.
   */
  CanPayload(const twai_message_t& twai_message);

  /*
   * Copy the data to the specified destination.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * destination       Pointer to the output buffer. Cannot be NULL.
   *                   The buffer must be large enough to receive all
   *                   available data.
   *
   * Returns: the number of bytes copied.
   *
   * TODO(emintz): Replace with memory-safe method.
   */
  size_t copy_payload_to(void *destination) const {
    memcpy(destination, twai_message.data, twai_message.data_length_code);
    return twai_message.data_length_code;
  }

  /*
   * Set the message identifier.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * id                Message identifier. Value must be in (0 .. 2^29).
   *                   The id indicates the message's data.
   *                   This is not checked.
   */
  CanPayload& set_id(int id);

  /*
   * Set the message payload. This method is memory-safe.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * data              Payload data. Cannot be NULL and must contain
   *                   at most TWAI_FRAME_MAX_DLC bytes.
   */
  template <typename T> CanPayload& set_data(const T& data) {
    static_assert(sizeof(T) <= TWAI_FRAME_MAX_DLC, "Data is too big to fit.");
    memcpy(twai_message.data, &data, twai_message.data_length_code = sizeof(T));
    return *this;
  }

  /*
   * Set the message payload.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * size              Payload length, in the range [0 .. 8]. This
   *                   is not checked; violate at your peril.
   * data              Payload data. Cannot be NULL and must contain
   *                   at least size bytes.
   */
  CanPayload& set_data(size_t size, const void *data) {
    memcpy(twai_message.data, data, twai_message.data_length_code = size);
    return *this;
  }


  /**
   * Returns a reference to the specified payload byte. This variant
   * is used to set data bytes.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * index             Byte number to return, in the range [0 .. this->size).
   *                   Not checked. Violate at your peril.
   */
  inline uint8_t& operator[] (size_t index) {
    return twai_message.data[index];
  }

  /**
   * Returns a const reference to the specified payload byte. This varialt
   * is used to read data bytes.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * index             Byte number to return, in the range [0 .. this->size).
   *                   Not checked. Violate at your peril.
   */
  inline const uint8_t& operator[] (size_t index) const {
    return twai_message.data[index];
  }

  /*
   * Returns the payload size.
   */
 inline size_t payload_size(void) const {
    return twai_message.data_length_code;
  }

  /*
   * Returns the message ID, a.k.a. message type. If the
   * message ID is too large to fit in 11 bits, send the
   * message in extended format.
   */
  inline int message_id(void) const {
    return twai_message.identifier;
  }

  /*
   * Include this node as a message recipient. If this method
   * is not invoked, this node will not receive it. This is
   * generally used for self-test.
   */
  inline CanPayload& send_to_self(void) {
    twai_message.self = 1;
    return *this;
  }

  /*
   * Do not retry a failed transmission. the CAN bus retries failed
   * transmissions unless otherwise directed.
   */
  inline CanPayload& no_retry(void) {
    twai_message.ss = 1;
    return *this;
  }

  /*
   * Set the payload length. Used in combination with operator[]
   * to set the payload.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * size              Payload size, a value in [0 .. 8]. This is not
   *                   checked. Be sure that the data bytes at offsets
   *                   [0 .. size - 1] are set! This, too, is not checked.
   *                   Y'all be careful, hear?
   */
  inline void set_payload_size(int size) {
    twai_message.data_length_code = size;
  }
};

#endif /* LIBRARIES_CANBUS_SRC_CANPAYLOAD_H_ */
