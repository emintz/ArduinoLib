/*
 * ToFlash32Persister.cpp
 *
 *  Created on: Apr 14, 2025
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

#include <src/ToFlash32Persister.h>

ToFlash32Persister::ToFlash32Persister() {
}

ToFlash32Persister::~ToFlash32Persister() {
}

bool ToFlash32Persister::operator() (
        const DataFieldConfig& field,
        Flash32Namespace& flash_memory,
        PersistStatus& errors) const {
  return errors.verify(
      save(
          field.get_id().c_str(),
          field.get_value().c_str(),
          flash_memory),
      field);
}
