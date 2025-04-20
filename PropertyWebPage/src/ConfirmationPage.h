/*
 * SubmitPage.h
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

#ifndef CONFIRMATIONPAGE_H_
#define CONFIRMATIONPAGE_H_

#include <string>

#include "WebPage.h"

class FieldLayout;
class ServerStatus;

class ConfirmationPage : public WebPage {

  /*
   * Loads values into the confirmation page.
   */
  void pre_process(
      WebServer& server,
      HTTPMethod request_method,
      String request_uri);

  /**
   * Generates the confirmation page
   */
  std::string html(void);

public:
  ConfirmationPage(
      ServerStatus& status,
      FieldLayout& layout,
      std::string header);
  virtual ~ConfirmationPage();

  virtual bool handle(
        WebServer &server,
        HTTPMethod requestMethod,
        const String &requestUri) override;
};

#endif /* CONFIRMATIONPAGE_H_ */
