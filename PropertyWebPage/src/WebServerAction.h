/*
 * WebServerAction.h
 *
 *  Created on: Feb 13, 2024
 *      Author: Eric Mintz
 *
 * TaskAction that runs a WebServer. This class must run under a TaskWithAction
 * or TaskWithActionH.
 *
 * Waits for a user to press and hold a button for a specified duration.
 * For the press and hold to be detected, the user must press the button
 * within a specified timeout. The button must be wired between the specified
 * pin and ground.
 *
 * This class blocks. Use3 it during setup only.
 *
 * Copyright (C) 2024 Eric Mintz
 * All Rights Reserved
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
 *
 */

#ifndef WEBSERVERACTION_H_
#define WEBSERVERACTION_H_

#include "Arduino.h"
#include "TaskAction.h"
#include "WebServer.h"

class WebServerAction : public TaskAction {
  WebServer& web_server;
public:
  /*
   * Constructor
   *
   * Parameters:
   *
   * +============+=======================================================+
   * | Name       | Contents                                              |
   * +============+=======================================================+
   * | web_server | The web server to run. Be sure to install a handler   |
   * |            | and start the the web server before starting the      |
   * |            | containing task.                                      |
   * +------------+-------------------------------------------------------+
   */
  WebServerAction(WebServer& web_server);
  virtual ~WebServerAction();

  /*
   * Task loop
   */
  virtual void run(void);
};

#endif /* WEBSERVERACTION_H_ */
