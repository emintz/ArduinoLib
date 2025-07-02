/*
 * CanPayloadAction.h
 *
 *  Created on: May 26, 2025
 *      Author: Eric Mintz
 *
 * Task action that handles incoming messages.
 *
 * Copyright (C) 2025 Eric Mintz
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
 */

#ifndef LIBRARIES_CANBUS_CANPAYLOADACTION_H_
#define LIBRARIES_CANBUS_CANPAYLOADACTION_H_

#include "CanPayload.h"
#include "CanPayloadHandler.h"
#include "MutexH.h"
#include "TaskAction.h"

#include "driver/twai.h"

class CanBus;

class CanPayloadAction : public TaskAction {
public:
  enum class State { CREATED, RUNNING, STOPPED };

private:
  CanBus& bus;
  CanPayloadHandler& handler;

  MutexH state_access;  // Must hold this to access the running flag.
  bool can_run;         // The running flag.
  State state;

  State get_state(void);

  void set_state(State state);

  void signal_loop_exit(void);

public:
  CanPayloadAction(
      CanBus& bus,
      CanPayloadHandler& handler);
  virtual ~CanPayloadAction();

  /*
   * Run action logic until stop() is invoked. The run logic
   * polls the bus for input, and forwards received input to the
   * application-provided CanPayloadHandler that was bound during
   * construction.
   */
  virtual void run(void);

  /*
   * Return true if and only if the handler is running and able to
   * process delivered messages. When the invocation returns false,
   * the handler has stoped accepting incoming messages, but the
   * containing task could still be running. Does not check the CAN controller
   * state, which can be retrieved by invoking CanBus::get_receive_status().
   * This method is thread-safe.
   *
   * Note: Since the handler enters the STOP state immediately BEFORE
   *       it terminates the containing, there will be a slight delay
   *       between the time it indicates that it has stopped and
   *       the task is actually terminated.
   *       If the invoking method requires actual task termination,
   *       it is recommended that it wait for a few microseconds to
   *       ensure that the task completely shuts down. This should not
   *       be a problem if the containing task has higher priority than
   *       the invoker.
   *
   */
  bool running(void);

  /*
   * Returns the run/stop flag. If true, the action is running. If
   * false, the action is stopping. This can be used to control
   * another task having the same life cycle.
   */
  inline bool run_stop_flag(void) const { return can_run; }

  /*
   * Signal this action to stop, then wait for it to
   * do so. This method is thread-safe.
   *
   * Note that the loop might be running when can_run
   * is false. Since run() changes the state to State:STOPPED
   * after the loop exits, querying the state precisely indicates
   * the run/stop state.
   *
   * Note: This method blocks until the handler stops polling for
   *       input. If the processor is currently handling a message, the
   *       loop will stop when processing completes.
   *       If a the pending input request delivers a message, it will
   *       be processed before the polling loop exits. Otherwise the
   *       polling loop will exit when the pending input request
   *       times out.
   *
   *       Since the state changes immediately BEFORE the containing
   *       task is terminated, it is recommended that the invoker
   *       wait for a few microseconds to allow the task complexly
   *       shuts down.
   *
   * Note: Since the handler enters the STOP state immediately BEFORE
   *       it terminates the containing, there will be a slight delay
   *       between the time it indicates that it has stopped and
   *       the task is actually terminated.
   *       If the invoking method requires actual task termination,
   *       it is recommended that it wait for a few microseconds to
   *       ensure that the task completely shuts down. This should not
   *       be a problem if the containing task has higher priority than
   *       the invoker.
   */
  void stop(void);
};

#endif /* LIBRARIES_CANBUS_CANPAYLOADACTION_H_ */
