/*
 * CalAlertHandlers.cpp
 *
 *  Created on: Jun 2, 2025
 *      Author: Eric Mintz
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

#include "CanAlertHandlers.h"
#include "CanAlertHandler.h"

CanAlertHandlers CanAlertHandlers::EMPTY;

CanAlertHandlers& CanAlertHandlers::add_alert_handler (
    uint32_t alerts, CanAlertHandler& handler){
  if (alerts) {
    this->active_alerts |= alerts;
    handlers_for_alerts.emplace_back(alerts, handler);
  }

  return *this;
}

void CanAlertHandlers::forward_alerts(uint32_t alerts, CanBus& can_bus) {
  for (std::vector<HandlerForAlerts>::iterator iter =
      handlers_for_alerts.begin();
    iter != handlers_for_alerts.end();
    iter++) {
    uint32_t triggered_alerts = iter->get_alerts() & active_alerts;
    if (triggered_alerts) {
      iter->get_handler()(can_bus, triggered_alerts);
    }
  }
}
