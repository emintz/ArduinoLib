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

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "FieldLayout.h"
#include "PropertyValidator.h"
#include "WebServer.h"

#include <list>
#include <functional>
#include <memory>
#include <string>

class WebPage {
  FieldLayout& layout;
  std::string header;  // Page header
  std::string style;   // Page style. Defaults to ""

protected:
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

  /**
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
