/*
 * TurnLedOnFunction.h
 *
 *  Created on: Nov 29, 2023
 *      Author: Eric Mintz
 *
 * VoidFunction implementation that turns a specified LED ON.
 *
 * Copyright (C) 2023 Eric Mintz
 * All Rights Reserved
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
 *
 */

#ifndef TURNLEDONFUNCTION_H_
#define TURNLEDONFUNCTION_H_

#include "Arduino.h"
#include "VoidFunction.h"

class TurnLedOnFunction : public VoidFunction {
  uint8_t pin_no;

public:
  TurnLedOnFunction(uint8_t pin_no);
  virtual ~TurnLedOnFunction();

  virtual void apply(void);
};

#endif /* TURNLEDONFUNCTION_H_ */
