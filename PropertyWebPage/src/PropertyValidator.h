/*
 * PropertyValidator.h
 *
 *  Created on: Feb 7, 2025
 *      Author: Eric Mintz
 *
 * Base class for property value validation. This is a hook for
 * future expansion.
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

#ifndef PROPERTYVALIDATOR_H_
#define PROPERTYVALIDATOR_H_

#include <string>

class PropertyValidator {

  std::string error_message;

public:

  /*
   * Constructor
   *
   * Creates a PropertyValidator instance that is bound to
   * a specified error message
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   *
   * error_message_c_str  The error message as a null-terminated
   *                      (i.e. C) string
   */
  PropertyValidator(
      const char *error_message_c_str);

  /*
   * Constructor
   *
   * Creates a PropertyValidator instance that is bound to
   * a specified error message
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   *
   * error_message        The error message as a string
   */
  PropertyValidator(
      const std::string error_message);
  virtual ~PropertyValidator();

  const std::string& get_error_message(void) const {
    return error_message;
  }

  /*
   * Validates the specified property value
   *
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * value                Property value to validate
   *
   * Returns: true if and only if the value is valid
   */
  virtual bool validate(const std::string& value) const = 0;
};

#endif /* PROPERTYVALIDATOR_H_ */
