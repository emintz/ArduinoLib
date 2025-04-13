/*
 * Int64Persister.h
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
 *
 * Persists a valid string representation of an int64_t value as an
 * int64_t.
 *
 */

#ifndef INT64PERSISTER_H_
#define INT64PERSISTER_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

class Int64Persister : public DataFieldFunction {
  Flash32Namespace& flash_namespace;
  PersistStatus& errors;
public:
  Int64Persister(
      Flash32Namespace& flash_namespace,
      PersistStatus& errors);
  virtual ~Int64Persister();

  virtual bool operator() (DataFieldConfig& field_config) const override;
};

#endif /* INT64PERSISTER_H_ */
