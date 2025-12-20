/*
 * ConfigurationCancelledPage.h
 *
 *  Created on: Dec 19, 2025
 *      Author: Eric Mintz
 *
 * Informs the user that the field practice session settings have not
 * been changed and stops the web server.
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

#ifndef LIBRARIES_PROPERTYWEBPAGE_CONFIGURATIONCANCELLEDPAGE_H_
#define LIBRARIES_PROPERTYWEBPAGE_CONFIGURATIONCANCELLEDPAGE_H_

#include "WebPage.h"

class FieldLayout;
class ServerStatus;

#include <string>

class ConfigurationCancelledPage: public WebPage {
public:
  ConfigurationCancelledPage(
      ServerStatus& status,
      FieldLayout& layout) :
        WebPage(status, layout, "") {
  }
  virtual ~ConfigurationCancelledPage() = default;

  virtual bool handle(
       WebServer &server,
       HTTPMethod requestMethod,
       const String &requestUri) override;
};

#endif /* LIBRARIES_PROPERTYWEBPAGE_CONFIGURATIONCANCELLEDPAGE_H_ */
