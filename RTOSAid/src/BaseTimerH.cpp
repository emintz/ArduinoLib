/*
 * BaseTimerH.cpp
 *
 *  Created on: Oct 16, 2025
 *      Author: Eric Mintz
 */

#include "BaseTimerH.h"

BaseTimerH::BaseTimerH(
    const char *name,
    VoidFunction& on_expiration,
    TickType_t default_timeout) :
      name_(name),
      on_expiration_(on_expiration),
      default_timeout_(default_timeout),
      h_timer_(NULL) {
}

BaseTimerH::~BaseTimerH() {
  if (h_timer_) {
    xTimerDelete(h_timer_, default_timeout_);
  }
  h_timer_ = NULL;
}

bool BaseTimerH::begin(BaseType_t auto_reload, TickType_t period) {
  h_timer_ = xTimerCreate(
      name_,
      period,
      auto_reload,
      this,
      timer_callback);
  return valid();

}

void BaseTimerH::on_timer_expired(void) {
    on_expiration_.apply();
}

void BaseTimerH::timer_callback(TimerHandle_t timer_handle) {
  static_cast<BaseTimerH *>(
      pvTimerGetTimerID(timer_handle)) -> on_timer_expired();
}

bool BaseTimerH::stop_from_isr(void) const {
  BaseType_t higher_priority_task_woken = pdFAIL;
  bool result = xTimerStopFromISR(h_timer_, &higher_priority_task_woken);
  if (higher_priority_task_woken) {
    portYIELD_FROM_ISR();
  }
  return result;
}
