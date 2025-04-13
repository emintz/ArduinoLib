/*
 * Int32Persister.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Eric Mintz
 *
 * Retrieves a signed 32-bit integer from EEPROM, converts it to its signed
 * base ten string equivalent, and sets the resulting value in a field
 * configuration.
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

#ifndef INT32PERSISTER_H_
#define INT32PERSISTER_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

class Int32Persister : public DataFieldFunction {
  Flash32Namespace& flash_namespace;
  PersistStatus& errors;
public:
  Int32Persister(
      Flash32Namespace& flash_namespace,
      PersistStatus& errors);
  virtual ~Int32Persister();

  virtual bool operator() (DataFieldConfig& field_config) const override;
};

#endif /* INT32PERSISTER_H_ */
