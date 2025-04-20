/*
 * DataTypeCharacteristics.h
 *
 *  Created on: Apr 11, 2025
 *      Author: Eric Mintz
 *
 * Provides a data type's name, persister, and retriever. The DataTypes
 * class provides an instance of each subtype; prefer it to creating
 * your own.
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

#include <map>
#include <memory>
#include <string>

class PersistenceAction;

class DataTypeCharacteristics {
  const std::string data_type_name;
  std::unique_ptr<PersistenceAction> to_flash_persister;
  std::unique_ptr<PersistenceAction> field_retriever;
  const std::map<const std::string, std::string> field_attributes;

public:
  /*
   * Constructor
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * name                 The data type name, e.g. "int32"
   * to_flash_persister   Saves the type to eeprom
   * retriever            Retrievers the type from eeprom
   * attributes           (name, value) pairs of field attributes to
   *                      be formatted "<name>=<value>" in the
   *                      generated HTML
   */
  DataTypeCharacteristics(
       const char *name,
       std::unique_ptr<PersistenceAction> to_flash_persister,
       std::unique_ptr<PersistenceAction> retriever,
       const std::map<const std::string, std::string>& attributes =
           std::map<const std::string, std::string>());

  virtual ~DataTypeCharacteristics();

  const std::map<const std::string, std::string>& attributes(void) const {
    return field_attributes;
  }

  PersistenceAction& persister(void) const {
    return *to_flash_persister;
  }

  PersistenceAction& retriever(void) const {
    return *field_retriever;
  }

  const std::string& type_name(void) const {
    return data_type_name;
  }
};

#endif /* DATATYPECHARACTERISTICS_H_ */
