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

#include "BaseTaskWithAction.h"
#include "ConfirmationTableRowGenerator.h"
#include "DataFieldConfig.h"
#include "DataTypes.h"
#include "Flash32.h"
#include "PersistFunction.h"
#include "PersistStatus.h"

#include <algorithm>
#include <vector>

static const char *redirect_to_home =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<h1>Redirecting to Home Page</h1>"
    "<meta http-equiv=\"refresh\" content=\"3;URL=/?noinit=true\" />"
    "<p>Will redirect to the home page in 3 seconds. "
    "If redirection fails, please click <a href=\"/\">here</a>.</p>"
    "</html>\n"
    ;

static const char *update_cancelled =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <h1>Cancelled</h1>\n"
    "  Cancelled by popular demand.\n"
    "</html>\n";

SaveOrReject::SaveOrReject(
    ServerStatus& status,
    Flash32Namespace& eeprom,
    FieldLayout& field_layout) :
        WebPage(status, field_layout, ""),
        eeprom(eeprom) {
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
      success();
    } else {
      std::string message("Invalid request: \"");
      message.append(user_command.c_str()).append("\".");
      server.send(500, "text/plain", message.c_str());
    }
  } else {
    server.send(500, "text/plain", "No user command");
    failure();
  }
  return status;
}

bool SaveOrReject::persist_values(WebServer &server) {
  PersistStatus errors(eeprom);
  PersistFunction persister(eeprom, errors);
  apply(persister);
  bool status = errors.status();
  if (status) {
    eeprom.commit();
    show_success(server);
  } else {
    show_errors(server,errors);
  }
  return status;
}

void SaveOrReject::show_errors(WebServer& server,const PersistStatus& errors) {
  std::string html(page_start);
  html.append("<h1>Persistence Failed</h1>\n")
    .append("<h2>The following errors occurred:</h2>\n")
    .append(format_errors(errors))
    .append("<br>\n").append("Web server stopped. Reboot and retry.\n")
    .append(page_end);
  server.send(
      500,
      "text/html",
      html.c_str());
  failure();
}

void SaveOrReject::show_success(WebServer& server) {
  std::string html(page_start);
  html.append(table_style)
      .append("<h1>Success!</h1>\n")
      .append("<h2>Values:</h2>\n")
      .append(table_start);
  {
    ConfirmationTableRowGenerator row_generator(html, 4);
    apply(row_generator);
  }
  html.append(table_end)
      .append("<br>\n<br>\nWeb server stopped.\n")
      .append(page_end);
  server.send(200, "text/html", html.c_str());
  success();
}
