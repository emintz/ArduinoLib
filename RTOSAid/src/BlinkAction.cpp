/*
 * BlinkAction.cpp
 *
 *  Created on: Jan 19, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2025 Eric Mintz
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

#include "BlinkAction.h"

BlinkAction::BlinkAction(
    const uint16_t led_pin,
    const uint16_t number_of_flashes,
    const uint16_t on_time_ms,
    const uint16_t off_time_ms,
    const uint16_t inter_group_wait_ms) :
      led_pin(led_pin),
      number_of_flashes(number_of_flashes),
      on_time_ms(on_time_ms),
      off_time_ms(off_time_ms),
      inter_group_wait_ms(inter_group_wait_ms) {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
}

BlinkAction::~BlinkAction() {
}

void BlinkAction::run(void) {
  while (true) {
    for (int i = 0; i < number_of_flashes; ++i) {
      digitalWrite(led_pin, HIGH);
      vTaskDelay(pdMS_TO_TICKS(on_time_ms));
      digitalWrite(led_pin, LOW);
      vTaskDelay(pdMS_TO_TICKS(off_time_ms));
    }
    vTaskDelay(pdMS_TO_TICKS(inter_group_wait_ms));
  }
}

void BlinkAction::blink_on(
    uint16_t number_of_flashes,
    uint16_t on_time_ms,
    uint16_t off_time_ms,
    uint16_t inter_group_wait_ms) {
  digitalWrite(led_pin, LOW);
  this->number_of_flashes = number_of_flashes;
  this->on_time_ms = on_time_ms;
  this->off_time_ms = off_time_ms;
  this->inter_group_wait_ms = inter_group_wait_ms;
  resume();
}

void BlinkAction::blink_off(void) {
  suspend();
  digitalWrite(led_pin, LOW);
}
