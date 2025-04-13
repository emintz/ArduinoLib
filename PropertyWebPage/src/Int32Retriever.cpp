/*
 * Int32Retriever.cpp
 *
 *  Created on: Apr 12, 2025
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

#include "Int32Retriever.h"

Int32Retriever::Int32Retriever(
    Flash32Namespace& flash_namespace,
    PersistStatus& errors) :
        flash_namespace(flash_namespace),
        errors(errors) {

}

Int32Retriever::~Int32Retriever() {
}

bool Int32Retriever::operator() (DataFieldConfig& field_config) const {
  bool status = true;
  int32_t integer_value = 0;
  field_config.set_value("");
  switch (auto field_status =
      flash_namespace.get_int32(
          field_config.get_id().c_str(),
          &integer_value)) {
    case Flash32Status::OK:
      field_config.set_value(std::to_string(integer_value));
      break;
    case Flash32Status::NOT_FOUND:
      field_config.set_value("");
      break;
    default:
      status = errors.verify(field_status, field_config);
  }

  return status;

}
