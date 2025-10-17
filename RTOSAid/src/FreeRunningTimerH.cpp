/*
 * FreeRunningTimerH.cpp
 *
 *  Created on: Oct 16, 2025
 *      Author: Eric Mintz
 */

#include "FreeRunningTimerH.h"

bool FreeRunningTimerH::begin(void)  {
  return
      BaseTimerH::begin(pdTRUE, cycle_time_)
      && pdPASS == xTimerStart(handle(), default_timeout());
}
