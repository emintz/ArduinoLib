/*
 * SaveConfiguration.cpp
 *
 *  Created on: Dec 20, 2025
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

#include "ConfirmationTableRowGenerator.h"
#include "SaveConfiguration.h"
#include "PersistFunction.h"
#include "PersistStatus.h"

#include <Flash32.h>


bool SaveConfiguration::persist_values(WebServer &server) {
  PersistStatus errors(eeprom);
  PersistFunction persister(eeprom, errors);
  apply(persister);
  bool status = errors.status();
  if (status) {
    eeprom.commit();
    show_success(server);
  } else {
    show_errors(server, errors);
  }
  return status;
}

void SaveConfiguration::show_errors(
    WebServer& server,const PersistStatus& errors) {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(500, "text/html", "");
  server.sendContent(page_start);
  server.sendContent(
      "<h1>Session Configuration Save Failed</h1>\n"
      "<h2>The following errors occurred:</h2>\n");
  server.sendContent(format_errors(errors).c_str());
  server.sendContent(
      "<br>\n"
      "Web server stopped. Reboot and retry.\n");
  server.sendContent(page_end);
  server.sendContent("");
  failure();
}

void SaveConfiguration::show_success(WebServer& server) {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent(page_start);
  server.sendContent(table_style);
  server.sendContent(
      "<h1>Practice Field Settings Saved Successfully</h1>\n"
      "<h2>Values:</h2>\n");
  server.sendContent(table_start);
  {
    // TODO: can we improve this?
    std::string html;
    ConfirmationTableRowGenerator row_generator(html, 4);
    apply(row_generator);
    server.sendContent(html.c_str());
  }
  server.sendContent(table_end);
  server.sendContent(
      "<br>\n<br>\n"
      "<p>The new practice field session settings are in effect.\n"
      "   The timer is starting.</p>\n"
      "<p>Good-bye</p>");
  server.sendContent(page_end);
  server.sendContent("");
  success();
}

bool SaveConfiguration::handle(
    WebServer &server,
    HTTPMethod requestMethod,
    const String &requestUri) {

  return persist_values(server);
}
