/*
 * DataEntryPage.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Eric Mintz
 *
 * Displays the page where the user enters property values, accepts the
 * values, and makes them available in the contained PropertyConfigurations
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

#ifndef DATAENTRYPAGE_H_
#define DATAENTRYPAGE_H_

#include "FieldLayout.h"
#include "WebPage.h"

#include <string>

class DataEntryPage : public WebPage {

  std::string html(void);

public:

  DataEntryPage(
      FieldLayout& layout,
      std::string header);

  virtual ~DataEntryPage();

  virtual bool handle(
        WebServer &server,
        HTTPMethod requestMethod,
        const String &requestUri) override;
};

#endif /* DATAENTRYPAGE_H_ */
