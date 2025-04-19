/*
 * Int32Retriever.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Eric Mintz
 *
 * Retrieves an int32_t value from EEProm, formats it as a decimal string,
 * and sets the resulting value in a field configuration.
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

#ifndef INT32RETRIEVER_H_
#define INT32RETRIEVER_H_

#include "PersistenceAction.h"

class DataFieldConfig;
class Flash32Namespace;
class PersistStatus;

class Int32Retriever :
    public PersistenceAction {
public:
  Int32Retriever();
  virtual ~Int32Retriever();

  virtual bool operator() (
      DataFieldConfig& field,
      Flash32Namespace& eeprom,
      PersistStatus& errors) override;
};

#endif /* INT32RETRIEVER_H_ */
