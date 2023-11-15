/*
 * Action.cpp
 *
 *  Created on: May 9, 2023
 *      Author: Eric Mintz
 */

#include "TaskAction.h"

#include "TaskWithAction.h"

TaskAction::TaskAction() :
  containing_task(NULL) {
}

TaskAction::~TaskAction() {
}

void TaskAction::delay_millis(uint32_t millis) {
  containing_task->delay_millis(millis);
}

void TaskAction::stop(void) {
  containing_task->stop();
}

void TaskAction::suspend(void) {
  containing_task->suspend();
}

uint32_t TaskAction::wait_for_notification(uint32_t millis) {
  return containing_task->wait_for_notification(millis);
}

void TaskAction::yield(void) {
  containing_task->yield();
}
