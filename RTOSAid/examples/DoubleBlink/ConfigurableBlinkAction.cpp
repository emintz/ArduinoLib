/**
 * BlinkAction.cpp
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 */

#include "Arduino.h"
#include "ConfigurableBlinkAction.h"

ConfigurableBlinkAction::ConfigurableBlinkAction(
    uint8_t led_pin,
    uint32_t on_off_time) :
    led_pin(led_pin),
    on_off_time(on_off_time) {
}

ConfigurableBlinkAction::~ConfigurableBlinkAction() {
}

void ConfigurableBlinkAction::run(void) {
  pinMode(led_pin, OUTPUT);

  // Endless task loop
  for (;;) {
    digitalWrite(led_pin, HIGH);
    delay_millis(on_off_time);  // Can also use delay()
    digitalWrite(led_pin, LOW);
    delay_millis(on_off_time);
  }
}
