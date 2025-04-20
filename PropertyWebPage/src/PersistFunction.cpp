/*
 * PersisFunction.cpp
 *
 *  Created on: Apr 15, 2025
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

#include "BasePersister.h"
#include "PersistFunction.h"
#include "DataFieldConfig.h"
#include "PersistenceAction.h"

PersistFunction::PersistFunction(
    Flash32Namespace& eeprom,
    PersistStatus& errors) :
        eeprom(eeprom),
        errors(errors) {
}

PersistFunction::~PersistFunction() {
}

bool PersistFunction::operator() (DataFieldConfig& field_config) {
  PersistenceAction& persister = field_config.get_persister();
  return persister(field_config, eeprom, errors);
}
