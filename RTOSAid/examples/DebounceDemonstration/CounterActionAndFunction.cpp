/*
 * GpioBounceCounter.cpp
 *
 *  Created on: Dec 3, 2023
 *      Author: Eric Mintz
 *
 */

#include "CounterActionAndFunction.h"

#define RED_LED_PIN 13
#define YELLOW_LED_PIN 14
#define TEST_PUSH_BUTTON_PIN 27

static uint8_t yellow_led_level = LOW;

CounterActionAndFunction::CounterActionAndFunction() :
    count(0) {
}

CounterActionAndFunction::~CounterActionAndFunction() {
}

void CounterActionAndFunction::apply(void) {
  // Make sure that run() cannot access count while we increment it.
  // Hold the mutex for the minimum possible time.
  {
    MutexLock lock(mutex);
    ++count;
  }
  digitalWrite(RED_LED_PIN, digitalRead(TEST_PUSH_BUTTON_PIN));
}

bool CounterActionAndFunction::begin(void) {
  return mutex.begin();
}

void CounterActionAndFunction::run(void) {
  uint32_t current_count = 0;
  for (;;) {
    delay_millis(15000);
    // Make sure that apply cannot change the count while we access it
    // by locking the mutex. To minimize the time we hold the lock, we
    // simply copy the count to a local variable for use after we release
    // the mutex.
    {
      MutexLock lock(mutex);
      current_count = count;
    }
    Serial.printf("Count is %lu.\n", count);
    yellow_led_level = (LOW == yellow_led_level) ? HIGH : LOW;
    digitalWrite(YELLOW_LED_PIN, yellow_led_level);
  }
}
