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

#import "CanAlertHandlers.h"
#import "CanApi.h"
#import "CanEnumerations.h"
#import "CanAlertAction.h"
#import "CanAlertHandlers.h"
#import "CanPayloadAction.h"
#import "MutexH.h"
#import "TaskWithActionH.h"

#import "driver/twai.h"
#import <memory>

class CanPayloadHandler;

class CanBus {

  friend CanPayloadAction;

  CanApi can_api;
  CanReceiveStatus receive_status;

  std::unique_ptr<CanPayloadAction> receive_action;
  std::unique_ptr<TaskWithActionH> receive_task;
  std::unique_ptr<CanAlertAction> alert_action;
  std::unique_ptr<TaskWithActionH> alert_task;

  MutexH status_mutex;

  /*
   * Clear any enqueued incoming messages.
   */
  CanBusOpStatus drain_input_queue(void);

  /*
   * Create and start the alert task if the caller provided alert
   * handlers. Otherwise do nothing. Note that the payload handling
   * task MUST be running when this is invoked.
   */
  CanBusOpStatus maybe_start_alert_task(
      CanAlertHandlers& alert_handlers);

  CanBusInitStatus really_init(void);

  CanBusDeinitStatus really_deinit(void);

  CanBusOpStatus really_start(
      CanPayloadHandler& payload_handler,
      CanAlertHandlers& alert_handlers);

  CanBusOpStatus really_stop(void);

  esp_err_t receive(twai_message_t& message, int wait_time_ms = 6000000) {
    return can_api.receive(message, wait_time_ms);
  }

  void set_receive_status(CanReceiveStatus receive_status);

  CanBusOpStatus really_transmit(
      const twai_message_t& message,
      int timeout_ms);

  /**
   * Stop the receive task if it is running, then delete
   * the receive task and the receive action.
   */
  void shut_down_receive_task(void);

  CanBusOpStatus start_bus(void);

  CanBusOpStatus start_receive_task(CanPayloadHandler& handler);

  CanBusOpStatus wait_for_bus_shutdown(void);

public:

  /*
   * Constructor. Creates an instance. The bus must be initialized and started
   * before it can be used.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * receive_pin       GPIO pin connected to the CAN bus adapter's RX. This
   *                   pin receives incoming data. See note.
   * transmit_pin      GPIO pin connected to the CAN bus adapter's TX. This
   *                   pin sends outgoing data. See note.
   * bits_per_second   Bus speed in bits per second. Note that all nodes
   *                   on a bus must be set to the same speed.
   * mode              Bus mode, defaults to normal -- send enabled and
   *                   all received messages are acknowledged.
   * bus_number        The physical CAN bus to use. The ESP32-S2 has
   *                   two buses, BUS_0 and BUS_1. Defaults to BUS_0.
   *
   * Returns: Start status. See enumeration for details.
   *
   * Note: check the ESP32 hardware manual for the number of supported
   *       CAN buses.
   *       Be sure that transmit_pin supports output. On the ESP32-S2,
   *       pins 34 - 39 are input-only and best avoided.
   */

  CanBus(
      uint8_t receive_pin,
      uint8_t transmit_pin,
      CanBusSpeed bits_per_second,
      CanBusMode mode = CanBusMode::NORMAL,
      CanBusNumber bus_number = CanBusNumber::BUS_0);
  virtual ~CanBus(void);

  static void begin(void);

  /*
   * Initializes the CAN bus driver. Has no effect if the bus is already
   * initialized. If this method succeeds, the bus must be started before
   * it can be used.
   */
  CanBusInitStatus init(void);

  /*
   * De-intializes the CAN bus. Has no effect if the bus is not running. Users
   * SHOULD disconnect from the bus before calling this.
   */
  CanBusDeinitStatus deinit(void);

  inline CanApi& get_can_api(void) {
    return can_api;
  }

  CanReceiveStatus get_receive_status(void);

  /*
   * Start bus recovery if the bus is off. Do nothing if the
   * bus is healthy. Invoking this method when the bus is healthy
   * is not considered erroneous.
   */
  CanBusOpStatus recover_if_bus_off(void);

  /*
   * Starts the bus. Note that the bus must be initialized. If the
   * invocation succeeds, the bus can send and receive messages.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * payload_handler   Invoked when a message arrives.
   * alert_handlers    Invoker provided alert processing. The default
   *                   instance is vacuous (i.e. does nothing).
   */
  CanBusOpStatus start(
      CanPayloadHandler& payload_handler,
      CanAlertHandlers& alert_handlers = CanAlertHandlers::EMPTY);

  /*
   * Stops the bus. Does nothing if the bus is already stopped.
   */
  CanBusOpStatus stop(void);

  /*
   * Transmit (i.e. send) the spepcified payload.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * payload           Message payload to transmit
   * timout_ms         Send timeout in milliseconds, the maximum time to
   *                   wait to transmit. The transmission fails if the
   *                   wait time exceeds this value.
   *
   * Note: an 11 bit ID is used if the payload ID fits, otherwise a 29 bit
   *       ID is used.
   */
  CanBusOpStatus transmit(const CanPayload& payload, int timeout_ms = -1);
};

#endif /* LIBRARIES_CANBUS_SRC_CANBUS_H_ */
