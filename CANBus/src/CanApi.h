/*
 * CanApi.h
 *
 *  Created on: May 28, 2025
 *      Author: Eric Mintz
 *
 * A thin wrapper around the ESP32 TWAI API that manages bus handles
 * and simplifies configuration. It formats status codes to simplify
 * debug logging (or at least to make log reading a bit less unpleasant),
 * and permits the validation of low-level functionality independently
 * from higher-level APIs.
 *
 * End users are welcome to invoke this library, but might
 * find higher level APIs more convenient. You pays your
 * nickel and takes your choice.
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

#ifndef CANAPI_H_
#define CANAPI_H_

#include "CanEnumerations.h"

#include "driver/twai.h"

class CanApi {
private:
  const uint8_t rx_pin;
  const uint8_t tx_pin;
  const CanBusSpeed speed;
  const uint8_t bus;
  twai_mode_t mode;

  twai_handle_t h_twai;  // To support V2 when come.
public:
  CanApi(
      uint8_t rx_pin,
      uint8_t tx_pin,
      CanBusSpeed speed = CanBusSpeed::BPS_100K,
      uint8_t bus = 0,
      CanBusMode can_bus_mode = CanBusMode::NORMAL);
  virtual ~CanApi();

  static void begin(void);
  static const char *to_cstr(esp_err_t error_code);
  static const char *to_cstr(twai_state_t state);

  /*
   * Retrieve the current bus status.
   */
  CanBusStatus bus_status(void);

  /*
   * Clear the receive queue, discarding all received messages
   * waiting for processing.
   */
  esp_err_t clear_receive_queue(void);

  /*
   * Clear the transmit queue, discarding all messages awaiting
   * transmissions.
   */
  esp_err_t clear_transmit_queue(void);

  esp_err_t install(void);

  esp_err_t read_alerts(uint32_t& alerts, int timeout_ms);

  esp_err_t read_status_info(twai_status_info_t& info);

  esp_err_t receive(twai_message_t& message, int timeout_ms);

  esp_err_t reconfigure_alerts(uint32_t alerts);

  /*
   * If the bus has halted (i.e. the bus state is TWAI_STATE_BUS_OFF),
   * start recovery. If the bus is healthy, do nothing. Fails if the
   * driver has not been installed.
   */
  esp_err_t recover_if_bus_off(void);

  esp_err_t send(const twai_message_t& message, int timeout_ms);

  esp_err_t start(void);

  esp_err_t stop(void);

  esp_err_t uninstall(void);
};

#endif /* CANAPI_H_ */
