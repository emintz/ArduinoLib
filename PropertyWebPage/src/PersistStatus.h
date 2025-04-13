/*
 * PersistStatus.h
 *
 *  Created on: Apr 9, 2025
 *      Author: Eric Mintz
 *
 * Tracks the status of persisting to flash memory. Instances accumulate
 * error messages. If an instance has no error messages, then everything
 * is OK.
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
 */

#ifndef PERSISTSTATUS_H_
#define PERSISTSTATUS_H_

#include "DataFieldConfig.h"
#include "Flash32.h"

#include <list>
#include <string>

class PersistStatus {
  Flash32BaseNamespace& flash_namespace;

  std::list<std::string> error_messages;

public:
  PersistStatus(Flash32BaseNamespace& flash_namespace);
  virtual ~PersistStatus();

  bool status(void) {
    return error_messages.empty();
  }

  void reset(void) {
    error_messages.clear();
  }

  /*
   * Verify the status and generate an error message if it flags a failure.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * status               Result of a Flash32 API invocation.
   * field_configuration  The that whose value was operated upon.
   *
   * Returns: true if the operation succeeded, false if it failed.
   *
   */
  bool verify(Flash32Status status, DataFieldConfig& field_configuration);
};

#endif /* PERSISTSTATUS_H_ */
