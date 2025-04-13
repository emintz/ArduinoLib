/*
 * FieldValueSetter.h
 *
 *  Created on: Mar 27, 2025
 *      Author: Eric Mintz
 *
 * Sets field values from a submitted form.
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

#ifndef FIELDVALUESETTER_H_
#define FIELDVALUESETTER_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "WebServer.h"

class FieldValueSetter : public DataFieldFunction {

  const WebServer& web_server;

public:
  /*
   * Creates an instance bound to the specified WebServer
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * web_server           Provides the field values
   */
  FieldValueSetter(const WebServer& web_server);

  virtual ~FieldValueSetter();

  /*
   * Takes a field identifier from the specified field_config,
   * sets the field_config value to the corresponding argument
   * in the web_server that is bound to this. Sets the value
   * to the empty string if no such argument exists.
   */
  virtual bool operator() (DataFieldConfig& field_config) const override;
};

#endif /* FIELDVALUESETTER_H_ */
