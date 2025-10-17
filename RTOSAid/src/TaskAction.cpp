/*
 * Action.cpp
 *
 *  Created on: May 9, 2023
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
 */

#include "TaskAction.h"

#include "TaskWithAction.h"

TaskAction::TaskAction() :
  containing_task(NULL) {
}

TaskAction::~TaskAction() {
}

void TaskAction::notify() {
  containing_task->notify();
}

void TaskAction::notify_from_isr() {
  return containing_task->notify_from_isr();
}

void TaskAction::delay_millis(uint32_t millis) {
  containing_task->delay_millis(millis);
}

void TaskAction::resume(void) {
  containing_task->resume();
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
