/*
 * TaskNotifierVoidFunction.h
 *
 *  Created on: May 19, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_TASKNOTIFIERVOIDFUNCTION_H_
#define SRC_TASKNOTIFIERVOIDFUNCTION_H_

class BaseTaskWithAction;

#include "VoidFunction.h"

class TaskNotifierVoidFunction : public VoidFunction {
  BaseTaskWithAction& task;

public:
  TaskNotifierVoidFunction(BaseTaskWithAction& task);
  virtual ~TaskNotifierVoidFunction();

  virtual void apply(void) override = 0;
};

#endif /* SRC_TASKNOTIFIERVOIDFUNCTION_H_ */
