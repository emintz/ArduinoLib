/**
 * TaskWithAction-based sketch that blinks the built in LED once
 * per second. This is the FreeRTOS analog of
 * https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/Blink/Blink.ino
 */
#include "Arduino.h"

#include "BlinkAction.h"
#include "TaskWithAction.h"

// The blink action is extremely simple so it doesn't need much stack storage.

static uint8_t blink_action_stack[1024];
static BlinkAction blink_action;
static TaskWithAction blink_task(
    "Blink",
    2,
    &blink_action,
    blink_action_stack,
    sizeof(blink_action_stack));

void setup() {
  Serial.begin(115200);
  // Print the sketch name, description, and when it was built.
   Serial.printf(
       "Blinky, simple blink sketch, built on %s at %s.\n",
       __DATE__,
       __TIME__);

  blink_task.start();
  Serial.println("Blink started.");
}

void loop() {
  // There's nothing to do here, so let's minimize CPU load by waiting for
  // the longest allowable time.
  vTaskDelay(portMAX_DELAY);
}
