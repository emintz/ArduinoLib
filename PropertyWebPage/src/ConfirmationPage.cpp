/*
 * SubmitPage.cpp
 *
 *  Created on: Mar 27, 2025
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

#include "ConfirmationPage.h"
#include "ConfirmationTableRowGenerator.h"
#include "FieldValueSetter.h"

static const char *confirmation_page_style =
    "<style>"
      "table, th, td {"
        "border: 1px solid black;"
        "border-collapse: collapse;"
      "}"
    "</style>";

static const char *confirmation_page_start =
    "<p>\n"
    "<table>\n"
      "<thead>\n"
        "<tr>"
          "<th>Setting</th>\n"
          "<th>Value</th>\n"
        "</tr>\n"
      "</thead>\n"
      "<body>\n"
    ;

// TODO: Better visuals, two buttons, Submit and Cancel instead of radio
//       buttons.
static const char *confirmation_page_end =
      "</body>\n"
    "</table>\n"
    "</p>\n"
    "<p>Would you like to accept or reject the foregoing configuration?</p>\n"
    "<p>"
    "<form action=\"/confirmation\">\n"
      "<p>"
        "<input type=\"radio\" id=\"accept\" name=\"confirm_config\" value=\"accept\" />\n"
        "<label for=\"accept\">Accept</label>\n"
      "</p>\n"
      "<p>\n"
        "<input type=\"radio\" id=\"reject\" name=\"confirm_config\" value=\"reject\" />\n"
        "<label for=\"reject\">Reject (default)</label>\n"
      "</p>\n"
      "<p>\n"
        "<input type=\"radio\" id=\"cancel\" name=\"confirm_config\" value=\"cancel\" />\n"
        "<label for=\"cancel\">Cancel</label>\n"
      "</p>\n"
      "<p>\n"
        "<input type=\"submit\" value=\"Accept or Reject\" />\n"
      "</p>\n"
    "</form>\n"
    "</p>\n"
    ;

ConfirmationPage::ConfirmationPage(
    FieldLayout& layout,
    std::string header) :
    WebPage(layout, header, confirmation_page_style) {
}

ConfirmationPage::~ConfirmationPage() {
}

bool ConfirmationPage::handle(
        WebServer &server,
        HTTPMethod request_method,
        const String &request_uri) {


  pre_process(server, request_method, request_uri);
  server.send(200, "text/html", html().c_str());

  return true;
}

std::string ConfirmationPage::html() {
  std::string page_html;
  append_style(page_html);
  append_header(page_html);
  page_html.append(confirmation_page_start);
  {
    ConfirmationTableRowGenerator generator(page_html, 4);
    apply(generator);
  }
  page_html.append(confirmation_page_end);
  Serial.println(page_html.c_str());
  return page_html;
}

void ConfirmationPage::pre_process(
    WebServer& server,
    HTTPMethod request_method,
    String request_uri) {
  FieldValueSetter field_setter(server);
  apply(field_setter);
}
