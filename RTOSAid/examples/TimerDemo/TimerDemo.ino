#include "Arduino.h"
#include "MicrosecondTimer.h"
#include "TurnLedOnFunction.h"

/*
 * TimerDemo.ino
 *
 * Created on: January 18, 2025
 *     Author: Eric Mintz
 *
 * Copyright (C) 2023 Eric Mintz
 * All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define BUILTIN_LED_PIN 2
#define RED_LED_PIN 13
#define YELLOW_LED_PIN 14

#define HALF_SECOND 500000L

static TurnLedOnFunction illuminate_builtin(BUILTIN_LED);
static TurnLedOnFunction illuminate_red(RED_LED_PIN);
static TurnLedOnFunction illuminate_yellow(YELLOW_LED_PIN);

static MicrosecondTimer builtin_led_timer("Built-in", &illuminate_builtin);
static MicrosecondTimer red_led_timer("Red", &illuminate_red);
static MicrosecondTimer yellow_led_timer("Yellow", &illuminate_yellow);

static bool turn_red_led_on = false;

static void all_leds_off(void) {
  digitalWrite(BUILTIN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

static void initialize_timer(const char *error_message, MicrosecondTimer *timer) {
  if (!timer->begin()) {
    Serial.println(error_message);
  }
}

static void start_timer(const char* error_message, MicrosecondTimer *timer) {
  if (!timer->start(HALF_SECOND)) {
    Serial.println(error_message);
  }
}

void setup(void) {
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  all_leds_off();
  initialize_timer("Built-in LED timer failure.", &builtin_led_timer);
  initialize_timer("Red LED timer failure", &red_led_timer);
  initialize_timer("Yellow LED timer failure", &yellow_led_timer);
}

void loop() {
  turn_red_led_on = !turn_red_led_on;
  all_leds_off();
  start_timer("Cannot start built-in timer.", &builtin_led_timer);
  start_timer("Cannot start red LED timer.", &red_led_timer);
  start_timer("Cannot start yellow LED timer.", &yellow_led_timer);

  delay(250);

 if (!turn_red_led_on) {
   red_led_timer.stop();
 }

 delay(750);
}
