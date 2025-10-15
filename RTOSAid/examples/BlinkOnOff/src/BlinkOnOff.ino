/**
 * BlinkOnOff.ino
 *
 *  Created on: Oct 15, 2025
 *      Author: Eric Mintz
 *
 *  Demonstrates how to start and stop blinking. To use this sketch
 *  you will need an ESP-32 development environment and the following
 *  hardware::
 *
 *  1. A prototyping board like an 830 pin solderless breadboard
 *  2. An ESP32 development board
 *  3. A breadboard-compatible momentary on push button switch. They
 *     are cheap and cheerful and belong in every hacker's parts bin.
 *  4. A 3 or 5 mm LED
 *  5. A 510 Ohm 1/4 Watt resistor
 *
 * To assemble the hardware:
 *
 * 1. Plug the ESP-32 development board into the prototyping board
 * 2. Connect the 3v3 pin to the power rail and a gnd pin to the ground
 *    rail.
 * 3. Mount the push button. Connect one side to ground and the other to
 *    GPIO pin 32.
 * 4. Mount the LED. Connect one end of the resistor to its cathode,
 *    the LED's short lead and the other end to ground. Connect the
 *    LED anode (long lead) to GPIO 23.
 * 5. Run a USB cable from your development computer to the ESP-32.
 *
 * Configure the sketch.
 *
 * 1. If your ESP-32 development board has a built-in LED, set
 *    BUILTIN_LED_PIN to its GPIO pin. It is  often wired to GPIO 2.
 *
 * Compile and upload the sketch. Enjoy!
 */

#include "Arduino.h"

#include "BlinkAction.h"
#include "TaskWithActionH.h"

#define BUILTIN_LED_PIN 2
#define INDICATOR_LED_PIN 23
#define BUTTON_PIN 32

static BlinkAction builtin_led_action(BUILTIN_LED_PIN, 1, 100, 100, 0);
static TaskWithActionH builtin_led_task(
    "Blink",
    2,
    &builtin_led_action,
    4096);

static bool blink_on;

void setup() {
  Serial.begin(115200);
  Serial.printf("Blink on/off sketch built on %s at %s.\n",
      __DATE__, __TIME__);
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(INDICATOR_LED_PIN, OUTPUT);

  for (int i = 0; i < 10; ++i) {
    digitalWrite(INDICATOR_LED_PIN, LOW);
    vTaskDelay(100);
    digitalWrite(INDICATOR_LED_PIN, HIGH);
    vTaskDelay(100);
  }

  builtin_led_task.start();
  blink_on = true;
}

void loop() {
  auto button_status = digitalRead(BUTTON_PIN);
  if (!blink_on && button_status == HIGH) {
    digitalWrite(INDICATOR_LED_PIN, HIGH);
    builtin_led_action.blink_on();
    blink_on = true;
  } else if (blink_on && button_status == LOW) {
    digitalWrite(INDICATOR_LED_PIN, LOW);
    builtin_led_action.blink_off();
    blink_on = false;
  }
  vTaskDelay(5);
}
