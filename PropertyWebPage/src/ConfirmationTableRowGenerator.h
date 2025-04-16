/*
 * ConfirmationFields.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Eric Mintz
 *
 * Generates fields in the confirmation table. Each row in the
 * emitted table contains a field name and its proposed value.
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

#ifndef CONFIRMATIONFIELDSGENERATOR_H_
#define CONFIRMATIONFIELDSGENERATOR_H_

#include "DataFieldFunction.h"
#include "DataFieldConfig.h"
#include <functional>
#include <string>

class ConfirmationTableRowGenerator : public DataFieldFunction {
  std::string& html;
  int indent;

public:
  ConfirmationTableRowGenerator(
      std::string& html,
      int indent);
  virtual ~ConfirmationTableRowGenerator();

  virtual bool operator() (DataFieldConfig& field_config) override;
};

#endif /* CONFIRMATIONFIELDSGENERATOR_H_ */
