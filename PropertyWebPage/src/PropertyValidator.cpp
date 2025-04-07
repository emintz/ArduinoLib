/*
 * PropertyValidator.cpp
 *
 *  Created on: Feb 7, 2025
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
 */

#include <src/PropertyValidator.h>

PropertyValidator::PropertyValidator(
    const char * error_message_c_str) :
      error_message(error_message_c_str) {
}

PropertyValidator::PropertyValidator(
    const std::string error_message) :
      error_message(error_message) {

}

PropertyValidator::~PropertyValidator() {
}

