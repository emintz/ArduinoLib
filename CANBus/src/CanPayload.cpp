/*
 * CanPayload.cpp
 *
 *  Created on: May 26, 2025
 *      Author: Eric Mintz
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

#import "CanPayload.h"

CanPayload::CanPayload() // :
//    id(0),
//    size(0)
    {
//  memset(data, 0, sizeof(data));
  memset(&twai_message, 0, sizeof(twai_message));
}

CanPayload::CanPayload(const twai_message_t& twai_message) // :
//  id(twai_message.identifier),
//  size(twai_message.data_length_code)
  {
//    memcpy(data, twai_message.data, size);
    memcpy(&(this->twai_message), &twai_message, sizeof(twai_message_t));
}

CanPayload& CanPayload::set_id(int id) {
//  this->id = id;
  twai_message.identifier = id;
  twai_message.extd =
      twai_message.identifier & 0b11111111111111111100000000000
      ? 1
      : 0;
  return *this;
}
