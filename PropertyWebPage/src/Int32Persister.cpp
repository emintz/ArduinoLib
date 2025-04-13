/*
 * Int32Persister.cpp
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

#include <src/Int32Persister.h>

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

#include <string>

Int32Persister::Int32Persister(
    Flash32Namespace& flash_namespace,
    PersistStatus& errors) :
        flash_namespace(flash_namespace),
        errors(errors) {
}

Int32Persister::~Int32Persister() {
}

bool Int32Persister::operator() (DataFieldConfig& field_config) const {
  int32_t value_to_persist = std::stoi(field_config.get_value());
  return errors.verify(
      flash_namespace.set_int32(
          field_config.get_id().c_str(),
          value_to_persist),
      field_config);
}
