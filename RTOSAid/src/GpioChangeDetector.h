/*
 * GpioChangeDetector.h
 *
 *  Created on: Dec 1, 2023
 *      Author: Eric Mintz
 *
 * Responds when the voltage on a GPIO changes.
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
 * The GPIO change detector classes implement pin change notification, allowing
 * code to run automatically when a GPIO input signal rises (changes from LOW
 * to HIGH), falls (changes from HIGH to LOW), or changes either way. The
 * code is "fire and forget". When a change handler is running, the application
 * can ignore the signal because the handler will run automatically when
 * the specified input change occurs on the specified GPIO pin.
 */

#ifndef GPIOCHANGEDETECTOR_H_
#define GPIOCHANGEDETECTOR_H_

#include "Arduino.h"

#include "VoidFunction.h"

/**
 * The supported change types.
 */
enum class GpioChangeType {
  LOW_TO_HIGH,  // Rising from LOW to HIGH
  HIGH_TO_LOW,  // Falling from HIGH to LOW
  ANY_CHANGE,   // Changing in any way
};


/**
 * Invokes a VoidFunction whenever the voltage changes on an input GPIO
 * pin. Note that callers must configure the pin, setting its mode to INPUT
 * and setting pullup or pulldown resistors as needed.
 *
 * Be aware that the invoked VoidFunction runs from an ISR, so it runs in
 * interrupt mode.
 */
class GpioChangeDetector final {
  gpio_num_t gpio_num;
  gpio_int_type_t interrupt_type;
  VoidFunction *on_pin_changed;

  /**
   * The low level interrupt handler that FreeRTOS invokes when the pin changes.
   * The method forwards the call to the GpioChangeDetector that is bound to
   * the triggering GPIO pin.
   *
   * Parameters
   *
   * Name   Contents
   * ------ ----------------------------------------------------------------
   * params A value having an unspecified type that, in this application,
   *        points to the GpioChangeDetector associated with the triggering
   *        pin
   */
  static void change_interrupt_handler(void *params);

  /**
   * Invokes the VoidFunction that is bound to this change detector. The
   * change_interrupt_handler() function invokes this method. Note that
   * handle_pin_change() and the invoked action are interrupt handers
   * and run in interrupt mode. Code accordingly.
   */
  void handle_pin_change(void);

public:

  /**
   * Creates and configures a GpioChangeDetector instance. The newly created
   * instance is inactive and will not respond to input. Invoke start() (see
   * below) to start watching the pin.
   *
   * Parameters:
   *
   * Name           Contents
   * -------------- ----------------------------------------------------------
   * pin_no         The GPIO pin to watch
   * change_type    The change to detect: LOW to HIGH, HIGH to LOW, or both.
   * on_pin_changed The VoidFunction to invoke when the pin changes. The
   *                function runs in interrupt mode. The function must return
   *                promptly, the quicker, the better. Be sure that the code
   *                is suitable for an ISR.
   */
  GpioChangeDetector(
      uint8_t pin_no,
      GpioChangeType change_type,
      VoidFunction *on_pin_changed);

  /**
   * Destructor: closes this GpioChangeDetector
   */
  virtual ~GpioChangeDetector();

  /**
   * Start watching the pin. If successful, the change notifier will invoke
   * on_pin_changed when the signal on the input pin undergoes the specified
   * change (LOW to HIGH, etc.) Make sure that the bound VoidFunction is
   * ready before invoking start().
   *
   * Returns: true if the change detector started successfully, false if
   * startup failed.
   */
  bool start(void);

  /**
   * Stops the change detector. When the function returns, the change detector
   * will no longer watch the pin and will not respond to signals.
   */
  void stop(void);
};

/**
 * Hardware GPIO change detector service that manages the change detector's
 * service's global state. Be sure to start the service before starting any
 * change detector. Callers must not create instances of this class. Use
 * the library's free for nothing GpioChangeService instance instead.
 *
 */

class HardwareGpioChangeService final {
  enum class Status {
    CLOSED,
    OPEN,
    FAILED,
  };

  Status status;
public:
  inline HardwareGpioChangeService(void) :
      status(Status::CLOSED) {
  }
  ~HardwareGpioChangeService();

  /**
   * Start the service. Be sure to invoke this function before starting
   * GpioChangeDetectors.
   *
   * Returns: true if the service started successfully, false otherwise.
   */
  bool begin(void);

  /**
   * Shuts the GPIO change detector service. Be sure to close all
   * GpioChangeService instances before invoking this function.
   */
  void end(void);
};

extern HardwareGpioChangeService GpioChangeService;

#endif /* GPIOCHANGEDETECTOR_H_ */
