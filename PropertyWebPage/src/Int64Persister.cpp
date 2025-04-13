/*
 * Int64Persister.cpp
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

#include "Int64Persister.h"

#include <string>

Int64Persister::Int64Persister(
    Flash32Namespace& flash_namespace,
    PersistStatus& errors) :
        flash_namespace(flash_namespace),
        errors(errors) {
}

Int64Persister::~Int64Persister() {
}

bool Int64Persister::operator() (DataFieldConfig& field_config) const {
  int64_t value_to_persist = std::stoll(field_config.get_value());
  return errors.verify(
      flash_namespace.set_int64(
              field_config.get_id().c_str(),
              value_to_persist),
          field_config);
}
