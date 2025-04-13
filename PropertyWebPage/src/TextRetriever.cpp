/*
 * TextRetriever.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: Eric Mintz
 *
 * Copyright (c) 2025, Eric Mintz
 * All Rights reserved.
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

#include "TextRetriever.h"

#include <cstring>

TextRetriever::TextRetriever(
    Flash32BaseNamespace& flash_namespace,
    PersistStatus& errors) :
        flash_namespace(flash_namespace),
        errors(errors) {
}

TextRetriever::~TextRetriever() {
}

bool TextRetriever::operator() (DataFieldConfig& config) const {
  char retrieved_value[64];
  size_t retrieved_byte_count = 0;
  std::memset(retrieved_value, '\0', sizeof(retrieved_value));

  bool status = true;
  config.set_value("");
  Flash32Status get_status = flash_namespace.get_str(
      config.get_id().c_str(),
      retrieved_value,
      sizeof(retrieved_value),
      &retrieved_byte_count);
  switch (get_status) {
    case Flash32Status::OK:
      config.set_value(retrieved_value);
      break;
    case Flash32Status::NOT_FOUND:
    default:
      status = errors.verify(get_status, config);
  }

  return status;
}
