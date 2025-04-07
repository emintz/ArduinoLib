/*
 * AcceptOrReject.h
 *
 *  Created on: Mar 29, 2025
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

#ifndef ACCEPTORREJECT_H_
#define ACCEPTORREJECT_H_

#include "BaseTaskWithAction.h"
#include "FieldLayout.h"
#import "WebPage.h"

class SaveOrReject : public WebPage {

  BaseTaskWithAction& waiting_task;

  bool persist_values(WebServer& server);

public:
  SaveOrReject(
      FieldLayout& layout,
      BaseTaskWithAction& waiting_task);
  virtual ~SaveOrReject();

  virtual bool handle(
      WebServer &server,
      HTTPMethod requestMethod,
      const String &requestUri) override;
};

#endif /* ACCEPTORREJECT_H_ */
