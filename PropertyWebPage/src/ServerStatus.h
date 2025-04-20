/*
 * ServerStatus.h
 *
 *  Created on: Apr 20, 2025
 *      Author: Eric Mintz
 *
 * The server status: used to stop the server when data entry is
 * complete and to indicate success or failure. The server must
 * keep running while the state is State::RUNNING.
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

#ifndef SERVERSTATUS_H_
#define SERVERSTATUS_H_

class ServerStatus {
public:
  enum class State {
    RUNNING,  // Server is running
    SUCCESS,  // Operation succeeded
    FAILURE,  // Operation failed
  };
private:
  State state;
public:
  ServerStatus();
  virtual ~ServerStatus();

  State operator() (void) const {
    return state;
  }

  void failure(void) {
    state = State::FAILURE;
  }

  void success(void) {
    state = State::SUCCESS;
  }
};

#endif /* SERVERSTATUS_H_ */
