/*
 * CanBus.h
 *
 *  Created on: May 24, 2025
 *      Author: Eric Mintz
 *
 * Controls the ESP32 CAN, its Controller Area Network bus. Note
 * that the library does not provide a default instance.
 *
 * The library does not support CLKOUT (clock output) or BUS_OFF, and
 * delegates device ID (as opposed to message ID) support to its
 * client.
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

#ifndef LIBRARIES_CANBUS_SRC_CANBUS_H_
#define LIBRARIES_CANBUS_SRC_CANBUS_H_

#include "CanEnumerations.h"
#include "CanPayloadAction.h"
#include "MutexH.h"
#include "TaskWithActionH.h"

#include "driver/twai.h"
#include <memory>

class CanPayloadHandler;

class CanBus {

  friend CanPayloadAction;

  twai_handle_t h_twai;
  CanBusStatus bus_status;
  CanReceiveStatus receive_status;

  std::unique_ptr<CanPayloadAction> receive_action;
  std::unique_ptr<TaskWithActionH> receive_task;

  MutexH status_mutex;

  CanBusInitStatus really_init(
      int bus_number,
      uint8_t receive_pin,
      uint8_t transmit_pin,
      CanBusSpeed bits_per_second,
      CanBusMode mode);

  CanBusDeinitStatus really_deinit(void);

  CanBusOpStatus really_start(CanPayloadHandler& handler);

  esp_err_t receive(twai_message_t& message) {
    return twai_receive_v2(h_twai, &message, portMAX_DELAY);
  }

  void set_receive_status(CanReceiveStatus receive_status);

  /**
   * Stop the receive task if it is running, then delete
   * the receive task and the receive action.
   */
  void shut_down_receive_task(void);

  CanBusOpStatus start_bus(void);

  CanBusOpStatus start_receive_task(CanPayloadHandler& handler);

public:
  CanBus();
  virtual ~CanBus();

  /*
   * Initializes the CAN bus driver. Has no effect if the bus is already
   * initialized. If this method succeeds, the bus must be started before
   * it can be used.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * bus_number        Bus to initialize, staring with 0. See note.
   * receive_pin       GPIO pin connected to the CAN bus adapter's RX. This
   *                   pin receives incoming data. See note.
   * transmit_pin      GPIO pin connected to the CAN bus adapter's TX. This
   *                   pin sends outgoing data. See note.
   * bits_per_second   Bus speed in bits per second. Note that all nodes
   *                   on a bus must be set to the same speed.
   * mode              Bus mode, defaults to normal -- send enabled and
   *                   all received messages are acknowledged.
   *
   * Returns: Start status. See enumeration for details.
   *
   * Note: check the ESP32 hardware manual for the number of supported
   *       CAN buses.
   *       Be sure that transmit_pin supports output. On the ESP32-S2,
   *       pins 34 - 39 are input-only and best avoided.
   */
  CanBusInitStatus init(
      int bus_number,
      uint8_t receive_pin,
      uint8_t transmit_pin,
      CanBusSpeed bits_per_second,
      CanBusMode mode = CanBusMode::NORMAL);

  /*
   * De-intializes the CAN bus. Has no effect if the bus is not running. Users
   * SHOULD disconnect from the bus before calling this.
   */
  CanBusDeinitStatus deinit(void);

  CanReceiveStatus get_receive_status(void);

  /*
   * Starts the bus. Note that the bus must be initialized. If the
   * invocation succeeds, the bus can send and receive messages.
      *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * handler           Invoked when a message arrives.
   */
  CanBusOpStatus start(CanPayloadHandler& handler);

  /*
   * Stops the bus.
   */
  CanBusOpStatus stop(void);


};

#endif /* LIBRARIES_CANBUS_SRC_CANBUS_H_ */
