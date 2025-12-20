/*
 * SaveConfiguration.h
 *
 *  Created on: Dec 20, 2025
 *      Author: Eric Mintz
 *
 * Copyright (c) 2025, Eric Mintz
 * All Rights reserved.
 *
 * Saves the field configuration settings, displays the "settings saved"
 * page, and stops the web server
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

#ifndef LIBRARIES_PROPERTYWEBPAGE_SAVECONFIGURATION_H_
#define LIBRARIES_PROPERTYWEBPAGE_SAVECONFIGURATION_H_

#include "WebPage.h"

class FieldConfiguration;
class Flash32Namespace;
class ServerStatus;

class SaveConfiguration: public WebPage {
  Flash32Namespace& eeprom;

  /*
   * Persist the field values, the value in the field's configuration
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * server               Web server that displays the generated
   *                      HTML.
   */
  bool persist_values(WebServer& server);

  /*
   * Show the error page
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * server               Web server that displays the generated
   *                      HTML.
   * errors               The errors to format as HTML.
   */
  void show_errors(WebServer& server, const PersistStatus& errors);

  /*
   * Show the success page
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * server               Web server that displays the generated
   *                      HTML.
   */
  void show_success(WebServer& server);

public:
  SaveConfiguration(
      ServerStatus& status,
      Flash32Namespace& eeprom,
      FieldLayout& layout) :
        WebPage(status, layout, ""),
        eeprom(eeprom) {
}

  virtual ~SaveConfiguration() = default;

  virtual bool handle(
      WebServer &server,
      HTTPMethod requestMethod,
      const String &requestUri) override;
};

#endif /* LIBRARIES_PROPERTYWEBPAGE_SAVECONFIGURATION_H_ */
