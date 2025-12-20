/*
 * RedirectToHome.h
 *
 *  Created on: Dec 20, 2025
 *      Author: Eric Mintz
 *
 * Saves the configuration or rejects it as directed by the user.
 * The user has the option of saving, correcting, or completely
 * rejecting changes.
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

#ifndef LIBRARIES_PROPERTYWEBPAGE_REDIRECTTOHOME_H_
#define LIBRARIES_PROPERTYWEBPAGE_REDIRECTTOHOME_H_

#include "WebPage.h"

class RedirectToHome: public WebPage {
  const char *landing_page;
public:
  RedirectToHome(
      ServerStatus& status,
      FieldLayout& layout,
      const char *landing_page) :
        WebPage(status, layout, ""),
        landing_page(landing_page) {
  }
  virtual ~RedirectToHome() = default;

  virtual bool handle(
      WebServer &server,
      HTTPMethod requestMethod,
      const String &requestUri) override;
};

#endif /* LIBRARIES_PROPERTYWEBPAGE_REDIRECTTOHOME_H_ */
