/*
 * PersisFunction.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Eric Mintz
 *
 * DataFieldFunction that persists a property value.
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

#ifndef PERSISTFUNCTION_H_
#define PERSISTFUNCTION_H_

#include "DataFieldFunction.h"

class Flash32Namespace;
class PersistStatus;

class PersistFunction : public DataFieldFunction {
  Flash32Namespace& eeprom;
  PersistStatus& errors;

public:
  PersistFunction(
      Flash32Namespace& eeprom,
      PersistStatus& errors);
  virtual ~PersistFunction();

  virtual bool operator() (DataFieldConfig& field_config);
};

#endif /* PERSISTFUNCTION_H_ */
