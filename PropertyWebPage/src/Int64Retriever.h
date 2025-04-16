/*
 * SignedLongRetriever.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Eric Mintz
 *
 * Retrieves a signed 64-bit integer from EEProm.
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

#ifndef INT64RETRIEVER_H_
#define INT64RETRIEVER_H_

#include "DataFieldFunction.h"
#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

class Int64Retriever : public DataFieldFunction {
  Flash32Namespace& flash_namespace;
  PersistStatus& errors;
public:
  Int64Retriever(
      Flash32Namespace& flash_namespace,
      PersistStatus& errors);
  virtual ~Int64Retriever();

  virtual bool operator() (DataFieldConfig& field_config) override;
};

#endif /* INT64RETRIEVER_H_ */
