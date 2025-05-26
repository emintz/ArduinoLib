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

class CanPayload final{
  int id;           // Message type identifier
  size_t size;      // Number of bytes, 0 - 8
  uint8_t data[8];  // Data. May be empty.
public:
  CanPayload();

  CanPayload(const twai_message_t& twai_message);

  CanPayload& set_id(int id) {
    this->id = id;
    return *this;
  }

  CanPayload& set_data(size_t size, const uint8_t *data) {
    memcpy(this->data, data, this->size = size);
    return *this;
  }

  const uint8_t operator[] (size_t index) {
    return data[index];
  }

  size_t payload_size(void) {
    return size;
  }

  int message_type(void) {
    return id;
  }
};

#endif /* LIBRARIES_CANBUS_SRC_CANPAYLOAD_H_ */
