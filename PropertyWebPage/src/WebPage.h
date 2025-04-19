/*
 * WebPage.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Eric Mintz
 *
 * Base web page class.
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

#ifndef WEBPAGE_H_
#define WEBPAGE_H_

#include "FieldLayout.h"
#include "WebServer.h"

class DataFieldFunction;
class PersistenceAction;
class Flash32Namespace;
class PersistStatus;

#include <string>

class WebPage {
  FieldLayout& layout;
  std::string header;  // Page header
  std::string style;   // Page style. Defaults to ""

protected:

  static const char *page_end;
  static const char *page_start;
  static const char *table_end;
  static const char *table_start;
  static const char *table_style;

  /*
   * Format errors as HTML.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * errors               The error messsages to format
   */
  static std::string format_errors(const PersistStatus& errors);

  /**
   * Constructor
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * layout               The fields to place on the web page.
   * header               Page header WITHOUT tags. May be empty.
   * style                Page style WITH tags. Note that the style
   *                      will be the first generated HTML.
   */
  WebPage(
      FieldLayout& layout,
      std::string header,
      std::string style = "");

  /*
   * Appends the specified string to the web page header.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * header               Text to append WITHOUT tags. May be empty.
   */
  WebPage& append_header(std::string& html);

  /*
   * Appends the page style to the specified html.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * html                 The formatted HTML to receive the page
   *                      style.
   */
  WebPage& append_style(std::string& html);

public:

  virtual ~WebPage();

  /*
   * Returns the field configuration that has the specified identifier
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * id                   Property configuration identifier
   */
  DataFieldConfig& field_configuration(std::string id);

  /*
   * Applies a data field function to this web spaces data fields
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * f                    The function to apply
   */
  void apply(DataFieldFunction& f) {
    layout.apply(f);
  }

  /*
   * Applies a persistence action to this web spaces data fields
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * action               The PersistenceAction to apply
   * eeprom               Non-volatile storage, holds property
   *                      values
   * errors               Receives any generated errors
   */
  void apply(
      PersistenceAction& action,
      Flash32Namespace& eeprom,
      PersistStatus& errors) {
    layout.apply(action, eeprom, errors);
  }

  /*
   * Returns the page's support for the specified HTTP method. The
   * default implementation only supports GET.
   */
  virtual bool can_handle(WebServer& web_server, HTTPMethod method);

  virtual bool handle(
      WebServer &server,
      HTTPMethod requestMethod,
      const String &requestUri) = 0;
};

#endif /* WEBPAGE_H_ */
