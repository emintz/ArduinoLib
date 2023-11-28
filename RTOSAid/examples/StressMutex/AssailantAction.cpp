/*
 * AssailantAction.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: Eric Mintz
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
 *
 */

#include "AssailantAction.h"
#include "RaceConditionStruct.h"
#include "TargetClass.h"
#include "TaskAction.h"

#define RED_LED_PIN 13

AssailantAction::AssailantAction(
    TargetClass *target,
    uint32_t delay,
    uint8_t led_pin) :
      target(target),
      delay(delay),
      led_pin(led_pin) {
}

AssailantAction::~AssailantAction() {
}

void AssailantAction::run(void) {
  RaceConditionStruct results;
  for (;;) {
    digitalWrite(led_pin, HIGH);
    target->have_a_go(delay, &results);
    digitalWrite(led_pin, LOW);
    if (results.entry_count + 1 != results.exit_count) {
      digitalWrite(RED_LED_PIN, HIGH);
    }
  }
}
