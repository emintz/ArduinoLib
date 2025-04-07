/*
 * AcceptOrReject.cpp
 *
 *  Created on: Mar 29, 2025
 *      Author: Eric Mintz
 *
 * Copyright (c) 2025, Eric Mintz
 * All Rights reserved.
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

#include "SaveOrReject.h"

static const char *redirect_to_home =
    "<h1>Redirecting to Home Page</h1>"
    "<meta http-equiv=\"refresh\" content=\"3;URL=/\" />"
    "<p>Will redirect to the home page in 3 seconds. "
    "If redirection fails, please click <a href=\"/\">here</a>.</p>"
    ;

static const char *update_cancelled =
    "<h1>Cancelled</h1>"
    "Cancelled by popular demand.";

SaveOrReject::SaveOrReject(
    FieldLayout& field_layout,
    BaseTaskWithAction& waiting_task) :
        WebPage(field_layout, "", ""),
        waiting_task(waiting_task) {
}

SaveOrReject::~SaveOrReject() {
}

bool SaveOrReject::handle(
    WebServer &server,
    HTTPMethod requestMethod,
    const String &requestUri) {
  bool status = true;
  if (server.hasArg("confirm_config")) {
    String user_command = server.arg("confirm_config");
    if (user_command == "accept") {
      status = persist_values(server);
    } else if (user_command == "reject") {
      server.send(200, "text/html", redirect_to_home);
    } else if (user_command == "cancel") {
      server.send(200, "text/html", update_cancelled);
      waiting_task.notify();
    } else {
      std::string message("Invalid request: \"");
      message.append(user_command.c_str());
      server.send(500, "text/plain", message.c_str());
      waiting_task.notify();
    }

  } else {
    server.send(500, "text/plain", "No user command");
  }
  return status;
}

bool SaveOrReject::persist_values(WebServer &server) {
  bool status = true;

  // TODO: persist to EEPROM
  server.send(
      503,
      "text/plain",
      "Persistence not implemented. Please try again later.");
  waiting_task.notify();
  return status;
}
