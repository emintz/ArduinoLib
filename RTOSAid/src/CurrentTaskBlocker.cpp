/*
 * CurrentTaskBlocker.cpp
 *
 *  Created on: Apr 20, 2025
 *      Author: Eric Mintz
 *
 * The layout of fields on a web page. Having this class saves memory by
 * allowing multiple web pages to use the same set of fields.
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty ofSeri
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "CurrentTaskBlocker.h"
#include "Arduino.h"

CurrentTaskBlocker::CurrentTaskBlocker() :
  // Sets h_invoking_task to the task handle of the invoking
  // task. This is why only the current task must invoke
  // notify().
  h_invoking_task(xTaskGetCurrentTaskHandle()) {
}

CurrentTaskBlocker::~CurrentTaskBlocker() {
  h_invoking_task = NULL;
}

void CurrentTaskBlocker::notify(void) {
  xTaskNotify(h_invoking_task, 1, eSetValueWithoutOverwrite);
}

void CurrentTaskBlocker::wait(void) {
  ulTaskNotifyTake(true, portMAX_DELAY);
}
