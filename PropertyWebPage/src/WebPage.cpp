/*
 * WebPage.cpp
 *
 *  Created on: Mar 4, 2025
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

#include <src/WebPage.h>

WebPage::WebPage(
    FieldLayout& layout,
    std::string header,
    std::string style) :
    layout(layout),
    style(style) {
  if (0 < header.length()) {
    (this->header = "<h1>")
        .append(header)
        .append("</h1>\n");
  } else {
    header = "";
  }
}

WebPage::~WebPage() {
}

WebPage& WebPage::append_header(std::string& html) {
  html.append(header);
  return *this;
}

WebPage& WebPage::append_style(std::string& html) {
  html.append(style);
  return *this;
}

bool WebPage::can_handle(WebServer& web_server, HTTPMethod method) {
  return method == HTTPMethod::HTTP_GET;
}
