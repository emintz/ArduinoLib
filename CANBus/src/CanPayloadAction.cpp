/*
 * CanPayloadAction.cpp
 *
 *  Created on: May 26, 2025
 *      Author: Eric Mintz
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

#import "CanPayloadAction.h"

#import "CanBus.h"
#import "CanBusMaps.h"
#import "CanPayloadHandler.h"
#import "MutexLock.h"

CanPayloadAction::State CanPayloadAction::get_state(void) {
  MutexLock lock(state_access);
  return state;
}

void CanPayloadAction::set_state(State state) {
  MutexLock lock(state_access);
  this->state = state;
}

void CanPayloadAction::signal_loop_exit(void) {
  MutexLock lock(state_access);
  can_run = false;
}

CanPayloadAction::CanPayloadAction(
    CanBus& bus,
    CanPayloadHandler& handler) :
        bus(bus),
        handler(handler),
        can_run(true),
        state(State::CREATED) {
  state_access.begin();
}

CanPayloadAction::~CanPayloadAction() {
}

void CanPayloadAction::run(void) {
  Serial.printf("Entered run(), can_run = %s.\n",
      can_run ? "true" : "false");
  bus.set_receive_status(CanReceiveStatus::RECEIVING);
  set_state(State::RUNNING);
  Serial.println("Running ...");
  while (can_run) {
    CanPayload payload;
    // Note that the expected shut down latency is half the
    // wait period in the following receive() invocation and
    // that the maximum possible latency is the wait period.
    esp_err_t receive_status = bus.receive(payload.as_twai_message(), 20);
    switch (receive_status) {
      case ESP_OK:
        handler(bus, payload);
        break;
      case ESP_ERR_TIMEOUT:
        // Nothing received. This is not an error, just
        // try again.
        break;
      default:
        Serial.printf("Exiting on receive status: %s (%d), can_run: %s.\n",
            CanBusMaps::INSTANCE.to_c_string(receive_status),
            receive_status,
            can_run ? "true" : "false");
        bus.set_receive_status(CanReceiveStatus::PANIC);
        signal_loop_exit();
        break;
    }
  }
  Serial.printf("Leaving run(), can_run = %s.\n",
      can_run ? "true" : "false");
  set_state(State::STOPPED);
  stop();
}

bool CanPayloadAction::running(void) {
  return State::RUNNING == get_state();
}

void CanPayloadAction::stop(void) {
  /*
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
  signal_loop_exit();
  while (State::RUNNING == get_state()) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  vTaskDelay(pdMS_TO_TICKS(100));
}
