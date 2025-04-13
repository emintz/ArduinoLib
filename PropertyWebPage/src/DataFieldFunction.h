/*
 * DattaFieldFunction.h
 *
 *  Created on: Apr 6, 2025
 *      Author: Eric Mintz
 *
 * Base class for functions that perform arbitrary actions on data field
 * configurations. We use them to generate HTML.
 * instances.
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

#ifndef DATAFIELDFUNCTION_H_
#define DATAFIELDFUNCTION_H_

class DataFieldConfig;

class DataFieldFunction {
public:
  DataFieldFunction();
  virtual ~DataFieldFunction();

  /**
   * Performs an arbitrary action on the specified field configuration.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * field_config         Contains the data used to generate the
   *                      HTML.
   */
  virtual bool operator() (DataFieldConfig& field_config) const = 0;
};

#endif /* DATAFIELDFUNCTION_H_ */
