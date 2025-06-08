/*
 * CalAlertHandlers.h
 *
 *  Created on: Jun 2, 2025
 *      Author: Eric Mintz
 *
 * Holds a vector of alert handlers and makes them available to
 * the alert processor.
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

#ifndef LIBRARIES_CANBUS_SRC_CANALERTHANDLERS_H_
#define LIBRARIES_CANBUS_SRC_CANALERTHANDLERS_H_

#import "Arduino.h"

#import <vector>

class CanAlertHandler;
class CanBus;

class CanAlertHandlers final {
  friend CanBus;

  class HandlerForAlerts {
    friend CanAlertHandlers;
    friend CanBus;

    uint32_t alerts;
    CanAlertHandler& handler;

  public:
    inline HandlerForAlerts(uint32_t alerts, CanAlertHandler& handler) :
      alerts(alerts),
      handler(handler) {}

    inline uint32_t get_alerts(void) const {
      return alerts;
    }

    inline CanAlertHandler& get_handler(void) const {
      return handler;
    }
  };

  uint32_t active_alerts;
  std::vector<HandlerForAlerts> handlers_for_alerts;

public:

  CanAlertHandlers() :
      active_alerts(0) {
  }

  /*
   * Add a handler for the specified alerts.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * alerts            Bit mask containing 1s for the alerts that the
   *                   handler responds to. SHOUD be non-zero. If alerts
   *                   is zero, the handler will never be invoked, so
   *                   the handler will not be added to the list and
   *                   the request is silently ignored.
   * handler           The CanAlertHandler subclass that responds to the
   *                   specified alerts.
   *
   * Returns: a reference to this instance to support chaining.
   *
   * Note: the alert processor forwards alerts to all handlers
   *       that accept it. Handlers are invoked in order of
   *       insertion.
   */
  CanAlertHandlers& add_alert_handler(
      uint32_t alerts, CanAlertHandler& handler);

  uint32_t get_active_alerts(void) const {
    return active_alerts;
  }

  /*
   * Forward the incoming alerts to any alert handler that
   * handles any of the active alerts.
   */
  void forward_alerts(uint32_t alerts, CanBus& can_bus);

  size_t size(void) const {
    return handlers_for_alerts.size();
  }

  /*
   * Contains no entries.
   */
  static CanAlertHandlers EMPTY;
};

#endif /* LIBRARIES_CANBUS_SRC_CANALERTHANDLERS_H_ */
