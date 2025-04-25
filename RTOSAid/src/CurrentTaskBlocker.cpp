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
  // task, meaning that invoking notify() or notify_from_isr()
  // will notify ONLY the currently running task (hence the class
  // name).
  h_invoking_task(xTaskGetCurrentTaskHandle()) {
}

CurrentTaskBlocker::~CurrentTaskBlocker() {
  h_invoking_task = NULL;
}

void CurrentTaskBlocker::notify(void) {
  xTaskNotify(h_invoking_task, 1, eSetValueWithoutOverwrite);
}

void IRAM_ATTR CurrentTaskBlocker::notify_from_isr(void) {
  BaseType_t higher_priority_task_woken;
  vTaskNotifyGiveFromISR(h_invoking_task, &higher_priority_task_woken);
  if (higher_priority_task_woken) {
     portYIELD_FROM_ISR();
   }
}

void CurrentTaskBlocker::wait(void) {
  ulTaskNotifyTake(true, portMAX_DELAY);
}
