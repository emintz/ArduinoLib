/*
 * ConfigurationCancelledPage.cpp
 *
 *  Created on: Dec 19, 2025
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

#include "ConfigurationCancelledPage.h"
#include "FieldLayout.h"
#include "ServerStatus.h"

#include <Flash32.h>

static const char *html =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <h1>Practice Field Session Settings Unchanged</h1>\n"
    "  <p>The practice field session settings have not been changed."
    "     The timer is starting.</p>\n"
    "  <p>Good-bye</p>\n"
    "</html>\n";

bool ConfigurationCancelledPage::handle(
    WebServer& server,
    HTTPMethod requestMethod,
    const String &requestUri) {
  server.send(200, "text/html", html);
  success();
  return true; // TODO
}
