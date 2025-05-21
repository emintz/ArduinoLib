/*
 * TaskNotifierVoidFunction.cpp
 *
 *  Created on: May 19, 2025
 *      Author: Eric Mintz
 */

#include "TaskNotifierVoidFunction.h"

#include "BaseTaskWithAction.h"

TaskNotifierVoidFunction::TaskNotifierVoidFunction(
    BaseTaskWithAction& task) :
    task(task) {

}

TaskNotifierVoidFunction::~TaskNotifierVoidFunction() {
}

void TaskNotifierVoidFunction::apply(void) {
  task.notify_from_isr();
}
