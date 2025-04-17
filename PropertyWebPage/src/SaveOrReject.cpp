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

#include "ConfirmationTableRowGenerator.h"
#include "DataFieldConfig.h"
#include "DataTypes.h"
#include "PersistFunction.h"

#include <algorithm>

static const char *redirect_to_home =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<h1>Redirecting to Home Page</h1>"
    "<meta http-equiv=\"refresh\" content=\"3;URL=/\" />"
    "<p>Will redirect to the home page in 3 seconds. "
    "If redirection fails, please click <a href=\"/\">here</a>.</p>"
    "</html>\n"
    ;

static const char *update_cancelled =
    "<html>\n"
    "  <h1>Cancelled</h1>\n"
    "  Cancelled by popular demand.\n"
    "</html>\n";

// TODO(emintz): the following duplicate HTML in ConfirmationPage.cpp.
//               Refactor.

static const char *confirmation_page_style =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<style>\n"
    "  table, th, td {\n"
    "    border: 1px solid black;\n"
    "    border-collapse: collapse;\n"
    "  }\n"
    "</style>\n";

static const char *confirmation_page_start =
    "<p>\n"
    "<table>\n"
    "  <thead>\n"
    "    <tr>\n"
    "      <th>Setting</th>\n"
    "      <th>Value</th>\n"
    "    </tr>\n"
    "  </thead>\n"
    "  <tbody>\n"
    ;

// TODO (emintz): The following is a substring of the confirmation
//                page end in ConfirmationPage.cpp. Refactor.
static const char *confirmation_page_end =
    "  </tbody>\n"
    "</table>\n";


SaveOrReject::SaveOrReject(
    FieldLayout& field_layout,
    Flash32Namespace& eeprom,
    BaseTaskWithAction& waiting_task) :
        WebPage(field_layout, "", ""),
        eeprom(eeprom),
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
    }

  } else {
    server.send(500, "text/plain", "No user command");
  }
  return status;
}

bool SaveOrReject::persist_values(WebServer &server) {
  PersistStatus errors(eeprom);
  PersistFunction persister(eeprom, errors);
  apply(persister);
  bool status = errors.status();
  if (status) {
    show_success(server);
  } else {
    show_errors(server,errors);
  }
  return status;
}

void SaveOrReject::show_errors(WebServer& server,const PersistStatus& errors) {
  std::string html("<h1>Persistence Failed</h1>\n");
  html.append("<h2>The following errors occurred:</h2>\n");
  const std::list<std::string> error_messages = errors.errors();
  for (
      auto message = error_messages.cbegin();
      message != error_messages.cend();
      ++message) {
    html.append(*message).append("<br>\n");
  }
  html.append("<br>\n").append("Web server stopped. Reboot and retry.\n");
  server.send(
      500,
      "text/html",
      html.c_str());
  // TODO(emintz): stop web server.
}

void SaveOrReject::show_success(WebServer& server) {
  std::string html(confirmation_page_style);
  html.append("<h1>Success!</h1>\n");
  html.append("<h2>Values:</h2>\n");
  html.append(confirmation_page_start);
  ConfirmationTableRowGenerator row_generator(html, 4);
  apply(row_generator);
  html.append(confirmation_page_end);
  html.append("<br>\n<br>\nWeb server stopped.\n");
  server.send(200, "text/html", html.c_str());
}
