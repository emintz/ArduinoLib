/**
 * BlinkAction.h
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 *
 *  Action that blinks an LED connected to a user-specified ESP32 GPIO
 *  pin. The user specifies the time to leave the LED on and off. Both
 *  values are set during construction.
 */

#ifndef CONFIGURABLEBLINKACTION_H_
#define CONFIGURABLEBLINKACTION_H_

#include "Arduino.h"

#include "TaskAction.h"

class ConfigurableBlinkAction: public TaskAction {

  const uint8_t led_pin;
  const uint32_t on_off_time;

public:
  /**
   * Constructor that sets the pin to blink and how long to hold the
   * connected LED on and off
   *
   * Parameters:
   *
   * Name          Contents
   * ------------- --------------------------------------------------------
   * led_pin       The ESP32 GPIO pin that controls the LED
   * on_off_time   The time to hold the LED on and off in milliseconds
   */
  ConfigurableBlinkAction(
      uint8_t led_pin,
      uint32_t on_off_time);
  virtual ~ConfigurableBlinkAction();

  /**
   * The code that blinks the LED.
   */
  virtual void run(void);
};

#endif /* CONFIGURABLEBLINKACTION_H_ */
