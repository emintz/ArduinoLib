/*
 * TextValidator.h
 *
 *  Created on: Feb 21, 2025
 *      Author: Eric Mintz
 *
 * Validates the contents of text fields. All characters must be
 * printable.
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

#ifndef TEXTVALIDATOR_H_
#define TEXTVALIDATOR_H_

#include "PropertyValidator.h"
#include <string>

class TextValidator : public PropertyValidator {
public:
  TextValidator(const std::string error_message_string);
  TextValidator(const char *error_message_c_str);
  virtual ~TextValidator();

  /**
   * Validates the specified property value
   *
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * value                Property value to validate
   *
   * Returns: true if and only if the value contains only printable
   *          values. Note that the empty string is vacuously valid.
   */
  virtual bool validate(const std::string& value) const;
};

#endif /* TEXTVALIDATOR_H_ */
