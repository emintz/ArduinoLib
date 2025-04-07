/*
 * AppendConfirmationFields.h
 *
 *  Created on: Mar 27, 2025
 *      Author: Eric Mintz
 *
 * Appends name/value pairs to the confirmation page's settings
 * table.
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

#ifndef APPENDCONFIRMATIONFIELDS_H_
#define APPENDCONFIRMATIONFIELDS_H_

#include "DataFieldConfig.h"

#include <functional>
#include <string>

class AppendConfirmationFields :
    public std::function<void(const DataFieldConfig&)> {

  std::string html;
  int indent;

public:
  AppendConfirmationFields(std::string& html, int indent);
  virtual ~AppendConfirmationFields();

  void operator()(const DataFieldConfig& field_config);
};

#endif /* APPENDCONFIRMATIONFIELDS_H_ */
