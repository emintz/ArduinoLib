/*
 * Mutex.cpp
 *
 *  Created on: May 10, 2023
 *      Author: Eric Mintz
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

#include "Arduino.h"

#include "Mutex.h"

#include "string.h"

Mutex::Mutex() {
  memset(&mutex_buffer, 0, sizeof(mutex_buffer));
}

Mutex::~Mutex() {
}

bool Mutex::begin(void) {
  return set_handle(xSemaphoreCreateMutexStatic(&mutex_buffer));
}
