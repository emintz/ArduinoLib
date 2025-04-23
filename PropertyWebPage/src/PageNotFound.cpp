/*
 * PageNotFound.cpp
 *
 *  Created on: Mar 31, 2025
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

#include "PageNotFound.h"

#include "CurrentTaskBlocker.h"
#include "FieldLayout.h"

PageNotFound::PageNotFound(ServerStatus& status, FieldLayout& layout) :
    WebPage(status, layout, "") {
}

PageNotFound::~PageNotFound() {
}

bool PageNotFound::handle(
    WebServer& server,
    HTTPMethod requestMethod,
    const String &requestUri) {
  std::string message("The URI: ");
  message += requestUri.c_str();
  message += " was not found.";
  server.send(404, "text/plain", message.c_str());
  failure();
  return true;
}
