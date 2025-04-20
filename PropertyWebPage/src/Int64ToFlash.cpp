/*
 * Int64ToFlash.cpp
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

#include "Int64ToFlash.h"

#include "Flash32.h"

#include <string>

Int64ToFlash::Int64ToFlash() {
}

Int64ToFlash::~Int64ToFlash() {
}

Flash32Status Int64ToFlash::save(
    const char *field_name,
    const char *field_value,
    Flash32Namespace& flash_memory) const {
  int64_t value_to_persist = std::stoll(field_value);
  return flash_memory.set_int64(
      field_name,
      value_to_persist);
}
