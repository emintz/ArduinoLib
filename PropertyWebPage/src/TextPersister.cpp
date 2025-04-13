/*
 * TextPersister.cpp
 *
 *  Created on: Apr 11, 2025
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

#include "TextPersister.h"

TextPersister::TextPersister(
    Flash32Namespace& flash_namespace,
    PersistStatus& errors) :
        flash_namespace(flash_namespace),
        errors(errors) {

}

TextPersister::~TextPersister() {
}

bool TextPersister::operator() (DataFieldConfig& field_config) const {
  bool status =
      errors.verify(
          flash_namespace.set_str(
              field_config.get_id().c_str(),
              field_config.get_value().c_str()),
          field_config);
  return status;
}
