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

#include "WebPage.h"
#include "PersistStatus.h"
#include "ServerStatus.h"

const char *WebPage::page_end = "</html>\n";

const char *WebPage::page_start =
    "<!DOCTYPE html>\n"
    "<html>\n";

const char *WebPage::table_end =
    "<p>\n"
    "<table>\n"
    "  <thead>\n"
    "    <tr>\n"
    "      <th>Setting</th>\n"
    "      <th>Value</th>\n"
    "    </tr>\n"
    "  </thead>\n"
    "  <tbody>\n";

const char *WebPage::table_start =
    "<p>\n"
    "<table>\n"
    "  <thead>\n"
    "    <tr>\n"
    "      <th>Setting</th>\n"
    "      <th>Value</th>\n"
    "    </tr>\n"
    "  </thead>\n"
    "  <tbody>\n";

const char *WebPage::table_style=
    "<style>\n"
    "  table, th, td {\n"
    "    border: 1px solid black;\n"
    "    border-collapse: collapse;\n"
    "  }\n"
    "</style>\n";

std::string WebPage::format_errors(const PersistStatus& errors) {
  std::string html;
  const auto error_messages = errors.errors();
  for (
      auto message = error_messages.cbegin();
      message != error_messages.cend();
      ++message) {
    html.append(*message).append("<br>\n");
  }
  return html;
}

WebPage::WebPage(
    ServerStatus& status,
    FieldLayout& layout,
    std::string header) :
        status(status),
        layout(layout) {
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

bool WebPage::can_handle(WebServer& web_server, HTTPMethod method) {
  return method == HTTPMethod::HTTP_GET;
}
