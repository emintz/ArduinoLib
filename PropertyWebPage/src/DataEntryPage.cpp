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
#include "FieldInitializer.h"
#include "Flash32.h"
#include "PersistStatus.h"

#include <Arduino.h>

static std::string data_entry_page_style(
    "  <style>\n"
    "    form  { display: table;      }\n"
    "    p     { display: table-row;  }\n"
    "    label { display: table-cell; }\n"
    "    input { display: table-cell; }\n"
    "  </style>\n");

static const char *cancel_form_and_buttons =
    "  <br/>\n"
    "  <form id='cancel-update' name='cancel-update' action='/confirmation'>\n"
    "    <p>\n"
    "      <input name='confirm_config'\n"
    "             id='confirm-config' readonly\n"
    "             accept-charset='utf-8'"
    "             hidden value='cancel'></input>\n"
    "    </p>\n"
    "  </form>\n"
    "  <br/>\n"
    "  <button type='submit' style='background-color:MediumSeaGreen' form='save-changes'>Submit</button>\n"
    "  <button type='submit' style='background-color:Red' form='cancel-update'>Cancel</button>\n"
    ;

DataEntryPage::DataEntryPage(
    ServerStatus& status,
    Flash32Namespace& eeprom,
    FieldLayout& layout,
    std::string header) :
    WebPage(status, layout, header),
    eeprom(eeprom) {
}

DataEntryPage::~DataEntryPage() {
}

bool DataEntryPage::handle(WebServer &server,
        HTTPMethod requestMethod,
        const String &requestUri) {
  PersistStatus errors(eeprom);
  if (!server.hasArg("noinit")) {
    load_values(errors);
  }
  if (errors.status()) {
    server.send(200, "text/html", data_entry_html().c_str());
  } else {
    server.send(500, "text/html", error_html(errors).c_str());
  }
  return true;
}

std::string DataEntryPage::data_entry_html() {
  std::string page_html;
  page_html.append(page_start);
  append_header(page_html);
  page_html.append(data_entry_page_style)
      .append("  <form name=\"save-changes\" id=\"save-changes\" accept-charset='utf-8' action=\"/save_changes\">\n");
  {
    DataEntryRowGenerator row_generator(page_html, 4);
    apply(row_generator);
  }
  page_html
      .append("  </form>\n")
      .append(cancel_form_and_buttons)
      .append(page_end);
  return page_html;
}

void DataEntryPage::load_values(PersistStatus& errors) {
  FieldInitializer initializer;
  apply(initializer, eeprom, errors);
}

std::string DataEntryPage::error_html(PersistStatus& errors) {
  std::string html(page_start);
  html.append("<h1>Could Not Initialize Field Values</h1\n")
      .append("<h2>The following errors occurred.</h2>\n")
      .append(format_errors(errors))
      .append("<br>\n")
      .append(page_end);
  Serial.println("Generated HTML:");
  Serial.println(html.c_str());
  return html;
}
