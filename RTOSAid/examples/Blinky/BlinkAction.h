/**
 * BlinkAction.h
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 *
 *  Action that blinks the ESP32 development board's built in LED
 *  every second. The blink is 500 milleseconds on, 500 milliseconds
 *  off.
 */

#ifndef BLINKACTION_H_
#define BLINKACTION_H_

#include "TaskAction.h"
#include "TaskWithAction.h"

class BlinkAction: public TaskAction {
public:
  BlinkAction();
  virtual ~BlinkAction();

  /**
   * The code that blinks the LED.
   */
  virtual void run(void);
};

#endif /* BLINKACTION_H_ */
