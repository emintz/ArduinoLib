/*
 * DataEntryFieldGenerator.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Eric Mintz
 *
 * Data field function that formats a row in the data entry table.
 * The row contains two fields: the entry's name, and the entry's value.
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

#ifndef DATAENTRYFIELDGENERATOR_H_
#define DATAENTRYFIELDGENERATOR_H_

#include "DataFieldFunction.h"
#include "DataFieldConfig.h"

#include <string>

class DataEntryRowGenerator : public DataFieldFunction {
  std::string& html;
  int indent;

public:
  DataEntryRowGenerator(
      std::string& html,
      int indent);
  virtual ~DataEntryRowGenerator();

  virtual bool operator() (DataFieldConfig& field_config);
};

#endif /* DATAENTRYFIELDGENERATOR_H_ */
