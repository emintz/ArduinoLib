/**
 * BlinkAction.cpp
 *
 *  Created on: Nov 16, 2023
 *      Author: Eric Mintz
 */

#include "Arduino.h"
#include "BlinkAction.h"

#define BUILTIN_LED_PIN 2  // Might be different on your board

BlinkAction::BlinkAction() {
}

BlinkAction::~BlinkAction() {
}

void BlinkAction::run(void) {
  // Set up: set the builtin LED pin to OUTPUT.
  pinMode(BUILTIN_LED_PIN, OUTPUT);

  // Endless task loop
  for (;;) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay_millis(500);  // Can also use delay()
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay_millis(500);
  }
}
