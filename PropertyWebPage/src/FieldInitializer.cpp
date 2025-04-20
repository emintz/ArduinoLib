/*
 * FieldInitializer.cpp
 *
 *  Created on: Apr 19, 2025
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

#include "PersistenceAction.h"

#include "Arduino.h"
#include "FieldInitializer.h"
#include "DataFieldConfig.h"

FieldInitializer::FieldInitializer() {
}

FieldInitializer::~FieldInitializer() {
}

bool FieldInitializer::operator() (
      DataFieldConfig& field,
      Flash32Namespace& eeprom,
      PersistStatus& errors)  {
  PersistenceAction& initializer = field.get_initializer();
  auto status = initializer(
      field,
      eeprom,
      errors);
  return status;
}
