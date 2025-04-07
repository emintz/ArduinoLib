/*
 * DataEntryPage.cpp
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

#include "DataEntryPage.h"
#include "DataEntryRowGenerator.h"

static std::string data_entry_page_style(
    "    <style>\n"
    "        form  { display: table;      }\n"
    "        p     { display: table-row;  }\n"
    "        label { display: table-cell; }\n"
    "        input { display: table-cell; }\n"
    "    </style>\n");

DataEntryPage::DataEntryPage(
    FieldLayout& layout,
    std::string header) :
    WebPage(layout, header, data_entry_page_style) {
}

DataEntryPage::~DataEntryPage() {
}

bool DataEntryPage::handle(WebServer &server,
        HTTPMethod requestMethod,
        const String &requestUri) {
  server.send(200, "text/html", html().c_str());

  return true;
}

std::string DataEntryPage::html() {
  std::string page_html;
  append_header(page_html);
  append_style(page_html);
  page_html.append("  <form action=\"/submit\">\n");
  {
    DataEntryRowGenerator row_generator(page_html, 4);
    apply(row_generator);
  }
  page_html
      .append("    <input type=\"submit\" value=\"Submit\" />\n")
      .append("  </form>\n");
  return page_html;
}
