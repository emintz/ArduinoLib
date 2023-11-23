/*
 * BlinkStatusAction.cpp
 *
 *  Created on: Nov 21, 2023
 *      Author: Eric Mintz
 */
#include "Arduino.h"

#include "BlinkStatusAction.h"
#include "PullQueueT.h"

#include <cstring>

BlinkStatusAction::BlinkStatusAction(
    uint8_t led_pin,
    PullQueueT<StatusBlinkCommand> *queue) :
      led_pin(led_pin),
      queue(queue) {
}

BlinkStatusAction::~BlinkStatusAction() {
}

void BlinkStatusAction::run(void) {
  StatusBlinkCommand command;
  std::memset(&command, 0, sizeof(command));
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  for (;;) {
    if (command.status_code) {
      show_status_code(
          command.on_time_millis,
          command.off_time_millis,
          command.wait_between_blink_sequence_millis,
          command.status_code);
    } else {
      wait_for_message();
    }
    queue->pull_message(&command);
  }
}

bool BlinkStatusAction::blink_once(
    uint32_t on_time_millis,
    uint32_t off_time_millis) {
  StatusBlinkCommand command;
  digitalWrite(led_pin, HIGH);
  bool stop_blinking = queue->peek_message(&command, on_time_millis);
  digitalWrite(led_pin, LOW);
  if (!stop_blinking) {
    stop_blinking = queue->peek_message(&command, off_time_millis);
  }
  return stop_blinking;
}

bool BlinkStatusAction::pause_between_blinks(
    uint32_t wait_between_blink_sequence_millis) {
  StatusBlinkCommand command;
  return queue->peek_message(&command, wait_between_blink_sequence_millis);
}

void BlinkStatusAction::show_status_code(
      uint32_t on_time_millis,
      uint32_t off_time_millis,
      uint32_t wait_between_blink_sequence_millis,
      uint16_t status_code) {
  bool stop_display = false;
  while (!stop_display) {
    for (
        uint16_t blink_no = 0;
        blink_no < status_code && !stop_display;
        ++blink_no) {
      stop_display = blink_once(on_time_millis, off_time_millis);
    }
    if (!stop_display) {
      stop_display = wait_for_message(wait_between_blink_sequence_millis);
    }
  }
}

void BlinkStatusAction::wait_for_message(void) {
  StatusBlinkCommand command;
  digitalWrite(led_pin, LOW);
  std::memset(&command, 0, sizeof(command));
  while (!queue->peek_message(&command)) {
  }
}

bool BlinkStatusAction::wait_for_message(uint32_t wait_time_millis) {
  StatusBlinkCommand command;
  return queue->peek_message(&command, wait_time_millis);
}
