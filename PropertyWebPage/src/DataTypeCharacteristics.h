/*
 * DataTypeCharacteristics.h
 *
 *  Created on: Apr 11, 2025
 *      Author: Eric Mintz
 *
 * Provides a data types name, persister, and retriever
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

#ifndef DATATYPECHARACTERISTICS_H_
#define DATATYPECHARACTERISTICS_H_

#include "DataFieldFunction.h"

#include <map>
#include <memory>
#include <string>

class DataTypeCharacteristics {
  const std::string data_type_name;
  std::unique_ptr<const DataFieldFunction> field_persister;
  std::unique_ptr<const DataFieldFunction> field_retriever;
  const std::map<const std::string, std::string> field_attributes;

public:
  DataTypeCharacteristics(
       const char *name,
       std::unique_ptr<const DataFieldFunction> persister,
       std::unique_ptr<const DataFieldFunction> retriever,
       const std::map<const std::string, std::string>& attributes =
           std::map<const std::string, std::string>());

  virtual ~DataTypeCharacteristics();

  const std::map<const std::string, std::string>& attributes(void) const {
    return field_attributes;
  }

  const DataFieldFunction& persister() const {
    return *field_persister;
  }

  const DataFieldFunction& retriever() const {
    return *field_retriever;
  }

  const std::string& type_name() const {
    return data_type_name;
  }
};

#endif /* DATATYPECHARACTERISTICS_H_ */
