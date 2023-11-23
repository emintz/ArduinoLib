/**
 * UseQueueAsTimer.ino
 *
 *  Created on: Nov 19, 2023
 *      Author: Eric Mintz
 *
 * PullQueueT<T>.peek_message() example
 */

#define BUILTIN_LED_PIN 2

#include "Arduino.h"

#include "BlinkStatusAction.h"
#include "PullQueueT.h"
#include "StatusBlinkCommand.h"
#include "TaskWithAction.h"

#include <cstring>

static StatusBlinkCommand pull_queue_storage[3];
static PullQueueT<StatusBlinkCommand> blink_command_queue(
    pull_queue_storage,
    3);

static uint8_t blink_action_stack[4096];
static BlinkStatusAction action(BUILTIN_LED_PIN, &blink_command_queue);
static TaskWithAction task(
    "Status",
    2,
    &action,
    blink_action_stack,
    sizeof(blink_action_stack));

static StatusBlinkCommand blink_messsage;

static StatusBlinkCommand fast_five = {
    100,
    100,
    500,
    5
};

static StatusBlinkCommand go_dark = {
    0,
    0,
    0,
    0
};

static StatusBlinkCommand slow_seven = {
    200,
    100,
    1000,
    7
};

void setup() {
   Serial.begin(115200);
   Serial.printf(
       "Blink status code sketch compiled on %s at %s.\n",
       __DATE__,
       __TIME__);
   std::memset(&blink_messsage, 0, sizeof(blink_messsage));

   blink_command_queue.begin();
   task.start();

}

// The loop function is called in an endless loop
void loop() {
  blink_command_queue.send_message(&fast_five);
  vTaskDelay(pdMS_TO_TICKS(15000));
  blink_command_queue.send_message(&go_dark);
  vTaskDelay(5000);
  blink_command_queue.send_message(&slow_seven);
  vTaskDelay(pdMS_TO_TICKS(10000));
}
