/*
 * VacuousDataFieldFunction.h
 *
 *  Created on: Apr 9, 2025
 *      Author: Eric Mintz
 *
 * A do-nothing DataFieldFunction.
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

#ifndef VACUOUSDATAFIELDFUNCTION_H_
#define VACUOUSDATAFIELDFUNCTION_H_

#include "DataFieldFunction.h"

class VacuousDataFieldFunction : public DataFieldFunction {
public:
  VacuousDataFieldFunction();
  virtual ~VacuousDataFieldFunction();

  virtual bool operator()(DataFieldConfig& field_config) override;
};

#endif /* VACUOUSDATAFIELDFUNCTION_H_ */
