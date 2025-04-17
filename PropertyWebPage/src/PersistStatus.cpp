/*
 * PersistStatus.cpp
 *
 *  Created on: Apr 9, 2025
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

#include "Arduino.h"

#include "PersistStatus.h"

PersistStatus::PersistStatus(Flash32BaseNamespace& flash_namespace) :
  flash_namespace(flash_namespace),
  error_messages() {
}

PersistStatus::~PersistStatus() {
}

bool PersistStatus::verify(
    Flash32Status status,
    const DataFieldConfig& field_configuration) {
  bool operation_succeeded = false;
  std::string error_message;

  switch (status) {
    case Flash32Status::OK:
      operation_succeeded = true;
      break;

    case Flash32Status::NOT_FOUND:
      error_message
          .append("The ")
          .append(field_configuration.get_id())
          .append(" field does not exist in the ")
          .append(flash_namespace.get_name())
          .append(" namespace.");
      break;

    case Flash32Status::NO_ROOM:
      error_message
          .append("There is no room in the ")
          .append(flash_namespace.get_name())
          .append(" namespace to store the ")
          .append(field_configuration.get_id())
          .append(" field.")
          ;
      break;

    case Flash32Status::INVALID_KEY:
      error_message
          .append(field_configuration.get_id())
          .append(" is an invalid field identifier.");
      break;

    case Flash32Status::CLOSED:
      error_message
          .append("The ")
          .append(flash_namespace.get_name())
          .append(" is closed. All operations are forbidden.");
      break;

    case Flash32Status::NOT_STARTED:
      error_message.append("The flash memory subsystem has not been started");
      break;

    case Flash32Status::FAILED:
      error_message
          .append("The operation on the ")
          .append(field_configuration.get_id())
          .append(" field has failed in the ")
          .append(flash_namespace.get_name())
          .append(" namespace.");
      break;

    default:
      error_message
          .append("The an unknown error occurred on the ")
          .append(field_configuration.get_id())
          .append(" field in the ")
          .append(flash_namespace.get_name())
          .append(" namespace.");
      break;
  }

  if (!operation_succeeded) {
    error_messages.push_back(error_message);
  }

  return operation_succeeded;
}
