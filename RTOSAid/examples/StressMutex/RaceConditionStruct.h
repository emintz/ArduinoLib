/*
 * RaceConditionStruct.h
 *
 *  Created on: Nov 23, 2023
 *      Author: Eric Mintz
 *
 * Contains data used to detect race conditions. When all is well,
 * the exit count will equal the entry count + 1.
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

#ifndef RACECONDITIONSTRUCT_H_
#define RACECONDITIONSTRUCT_H_

#include "Arduino.h"

typedef struct {
  uint32_t entry_count;
  uint32_t exit_count;

} RaceConditionStruct;

#endif /* RACECONDITIONSTRUCT_H_ */
