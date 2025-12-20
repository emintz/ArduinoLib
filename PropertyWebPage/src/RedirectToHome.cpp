/*
 * RedirectToHome.cpp
 *
 *  Created on: Dec 20, 2025
 *      Author: WebPage
 *
 * Saves the configuration or rejects it as directed by the user.
 * The user has the option of saving, correcting, or completely
 * rejecting changes.
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

#include "RedirectToHome.h"

static constexpr const char *redirect_page_start =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<h1>Returning to the Session Configuration Page</h1>"
    "<meta http-equiv='refresh' content='3;URL=";

static constexpr const char *redirect_page_middle =
    "' />\n"
    "<p>Will redirect to the home page in 3 seconds.\n"
    "If redirection fails, please click <a href='";

static constexpr const char *redirect_page_end =
    "'>here</a>.</p>"
    "</html>\n"
    ;

bool RedirectToHome::handle(
    WebServer &server,
    HTTPMethod requestMethod,
    const String &requestUri) {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent(redirect_page_start);
  server.sendContent(landing_page);
  server.sendContent(redirect_page_middle);
  server.sendContent(landing_page);
  server.sendContent(redirect_page_end);
  server.sendContent("");
  return true;
}
