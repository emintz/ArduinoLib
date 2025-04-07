/*
 * VacuousValidator.h
 *
 *  Created on: Feb 26, 2025
 *      Author: Eric Mintz
 *
 * A validator that accepts any input.
 *
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


#ifndef VACUOUSVALIDATOR_H_
#define VACUOUSVALIDATOR_H_

#include "PropertyValidator.h"

#include <string>

class VacuousValidator : public PropertyValidator {
public:
  VacuousValidator();
  virtual ~VacuousValidator();

  virtual bool validate(const std::string& value) const;
};

#endif /* VACUOUSVALIDATOR_H_ */
