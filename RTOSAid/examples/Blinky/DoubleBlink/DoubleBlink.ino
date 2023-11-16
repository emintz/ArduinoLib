/**
 * Blinks two LEDs, one with 500 millisecond on and off times, the other
 * with 333 millisecond on and off times. Note that this would be difficult
 * in a traditional Arduino sketch because 500, which is 2^2 * 5^3, is
 * relatively prime to 333, which is 3^2 * 37.
 */
#include "Arduino.h"

#include "ConfigurableBlinkAction.h"
#include "TaskWithAction.h"

#define BUILTIN_LED_PIN 2
#define RED_LED_PIN 13

uint8_t builtin_led_stack[1024];
ConfigurableBlinkAction builtin_action(BUILTIN_LED_PIN, 500);
TaskWithAction builtin_task(
    "BuiltinLED",
    2,
    &builtin_action,
    builtin_led_stack,
    sizeof(builtin_led_stack));

uint8_t red_led_stack[1024];
ConfigurableBlinkAction red_action(RED_LED_PIN, 333);
TaskWithAction red_task(
    "RedLED",
    2,
    &red_action,
    red_led_stack,
    sizeof(red_led_stack));

void setup() {
  Serial.begin(115200);
  Serial.printf(
      "DoubleBlink, sketch that blinks two LEDs simultaneously, built on %s at %s.\n",
      __DATE__,
      __TIME__);
  builtin_task.start();
  red_task.start();
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
