/*
 * DataFieldPersister.h
 *
 *  Created on: Apr 14, 2025
 *      Author: Eric Mintz
 *
 * Base class for functions that perform field value persistence-related
 * operations.
 *
 * TODO(emintz): implementation.
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

#ifndef DATAFIELDPERSISTANCE_H_
#define DATAFIELDPERSISTANCE_H_

class DataFieldConfig;
class Flash32Namespace;
class PersistStatus;

class DataFieldPersistance {
public:
  DataFieldPersistance();
  virtual ~DataFieldPersistance();

  /*
   * Performs an arbitrary action on a specified DataFieldConfig, posting
   * errors (if any) in the specified PersistStatus.
   *
   *    *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * field                The target of the action
   * eeprom               Non-Volatile storage
   * errors               Place error messages (if any) here.
   */
  virtual void operator() (
      DataFieldConfig& field,
      Flash32Namespace& eeprom,
      PersistStatus& errors);
};

#endif /* DATAFIELDPERSISTANCE_H_ */
