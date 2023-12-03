/*
 * GpioChangeDetector.cpp
 *
 *  Created on: Dec 1, 2023
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
 */

#include "GpioChangeDetector.h"

#include "driver/gpio.h"

HardwareGpioChangeService GpioChangeService;

static gpio_int_type_t to_gpio_int_type(GpioChangeType change_type) {
  gpio_int_type_t interrupt_type = GPIO_INTR_DISABLE;

  switch(change_type) {
  case GpioChangeType::LOW_TO_HIGH:
    interrupt_type = GPIO_INTR_POSEDGE;
    break;

  case GpioChangeType::HIGH_TO_LOW:
    interrupt_type = GPIO_INTR_NEGEDGE;
    break;

  case GpioChangeType::ANY_CHANGE:
    interrupt_type = GPIO_INTR_ANYEDGE;
    break;
  }
  return interrupt_type;
}

GpioChangeDetector::GpioChangeDetector(
    uint8_t pin_no,
    GpioChangeType change_type,
    VoidFunction *on_pin_changed) :
      gpio_num((gpio_num_t(pin_no))),
      interrupt_type(to_gpio_int_type(change_type)),
      on_pin_changed(on_pin_changed) {
}

GpioChangeDetector::~GpioChangeDetector() {
  stop();
}

void GpioChangeDetector::change_interrupt_handler(void *params) {
  static_cast<GpioChangeDetector *>(params)->handle_pin_change();
}

void GpioChangeDetector::handle_pin_change(void) {
  on_pin_changed->apply();
}

bool GpioChangeDetector::start(void) {
  return
      (gpio_set_intr_type(gpio_num, interrupt_type) == ESP_OK)
      && (gpio_isr_handler_add(gpio_num, change_interrupt_handler, this));
}

void GpioChangeDetector::stop(void) {
  gpio_set_intr_type(gpio_num, GPIO_INTR_DISABLE);
  gpio_isr_handler_remove(gpio_num);
}

HardwareGpioChangeService::~HardwareGpioChangeService() {
}

bool HardwareGpioChangeService::begin(void) {
  if (Status::CLOSED == status) {
    status = (gpio_install_isr_service(0) == ESP_OK)
        ? Status::OPEN
        : Status::FAILED;
  }
  return HardwareGpioChangeService::Status::OPEN == status;
}

void HardwareGpioChangeService::end(void) {
  gpio_uninstall_isr_service();
}
