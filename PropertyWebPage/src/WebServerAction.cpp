/*
 * WebServerAction.cpp
 *
 *  Created on: Feb 13, 2024
 *      Author: Eric Mintz
 *
 * Waits for a user to press and hold a button for a specified duration.
 * For the press and hold to be detected, the user must press the button
 * within a specified timeout. The button must be wired between the specified
 * pin and ground.
 *
 * This class blocks. Use3 it during setup only.
 *
 * Copyright (C) 2024 Eric Mintz
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

#include "Arduino.h"
#include "WebServerAction.h"

#include "CurrentTaskBlocker.h"
#include "ServerStatus.h"
#include "WebServer.h"

WebServerAction::WebServerAction(
    WebServer& web_server,
    ServerStatus& server_status,
    CurrentTaskBlocker& blocker) :
    web_server(web_server),
    status(server_status),
    blocker(blocker) {
}

WebServerAction::~WebServerAction() {
  Serial.println("WebServerAction destroyed.");
}

void WebServerAction::run(void) {
  Serial.println("Starting web server refresh.");
  bool notified = false;
  while (status() == ServerStatus::State::RUNNING) {
    web_server.handleClient();
    vTaskDelay(1);
  }
  blocker.notify();
  stop();
}
