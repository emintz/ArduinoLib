/*
 * FieldValueSetter.cpp
 *
 *  Created on: Mar 27, 2025
 *      Author: Eric Mintz
 *
 * Displays the page where the user enters property values, accepts the
 * values, and makes them available in the contained PropertyConfigurations
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

#include "FieldValueSetter.h"

FieldValueSetter::FieldValueSetter(const WebServer& web_server) :
    web_server(web_server) {
}

FieldValueSetter::~FieldValueSetter() {
}

void FieldValueSetter::operator() (DataFieldConfig& field_config) {
  String field_id(field_config.get_id().c_str());
  if (web_server.hasArg(field_id)) {
    const char *value_from_server = web_server.arg(field_id.c_str()).c_str();
    std::string field_value(value_from_server);
    field_config.set_value(web_server.arg(field_id).c_str());
  } else {
    field_config.set_value("");
  }
}

