/*
 * CanEnumerations.h
 *
 *  Created on: May 24, 2025
 *      Author: Eric Mintz
 *
 * Library-provided enumerations that hide the native CAN bus
 * library's complexities.
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

#ifndef LIBRARIES_CANBUS_SRC_CANENUMERATIONS_H_
#define LIBRARIES_CANBUS_SRC_CANENUMERATIONS_H_

/*
 * Initialization status.
 */
enum class CanBusInitStatus {
  SUCCEEDED,   // Bus initialized successfully.
  FAILED,      // Bus initialized failed.
  ALREADY_UP,  // Nothing done because the bus is
               // already initialized.
};

/*
 * De-initialization status.
 */
enum class CanBusDeinitStatus {
  SUCCEEDED,     // Bus shutdown successfully.
  FAILED,        // Bus shutdown failed.
  ALREADY_DOWN,  // Nothing done because the bus is
                 // already de-initialized.
  NOT_STOPPED,   // Cannot de-initialize because the bus
                 // is still carrying traffic.
};

enum class CanReceiveStatus {
  DOWN,      // Not started, not initialized, out to lunch
  RECEIVING, // Receiving messages
  PANIC,     // Panic stopped, unrecoverable error. Should
             // never happen.
};

/*
 * Operation status, returned from send, queue clear, etc.
 */
enum class CanBusOpStatus {
  SUCCEEDED,        // Operation succeeded, everything copesthetic.
  UNAVAILABLE,      // Not initialized or running
  INVALID_ARGUMENT, // Invalid argument, bad value or missing
  TIMEOUT,          // Could not complete operation within
                    // specified deadline
  TRANSMIT_FAILED,  // TX queue is disabled and another message
                    // is currently transmitting. Should never
                    // happen.
  CANNOT_SEND,      // Cannot send in listen-only mode
  MEMORY_FULL,      // No memory available
  INVALID_STATE,    // Operation not supported in the current
                    // bus state.
  FAILED,           // Operation-specific failure.
  UNKNOWN,          // Unknown, unexpected error
};

/*
 * CAN bus behavior, set when the bus is enabled.
 */
enum class CanBusMode {
  LURK,        // Lurk on the bus, receiving all messages.
  NORMAL,      // Receive and acknowledge messages; transmit
               // Normally. Use this in production.
  SELF_TEST,   // Do not acknowledge message receipt; transmit
               // normally. Use this when self testing
};

/*
 * Bus speed, set when bus is enabled. Note that BPS stands
 * for "Bits Per Second".
 */
enum class CanBusSpeed {
  // TODO(emintz): enable when supported
//  BPS_12_5K,    // 12.5 kilobits/second
//  BPS_16K,      // 16 kilobits/second
//  BPS_20K,      // 20 kilobits/second
  BPS_25K,      // 25 kilobits/second
  BPS_50K,      // 50 kilobits/second
  BPS_100K,     // 100 kilobits/second
  BPS_125K,     // 125 kilobits/second
  BPS_250K,     // 250 kilobits/second
  BPS_500K,     // 500 kilobits/second
  BPS_800K,     // 800 kilobits/second
  BPS_1M,       // 1 megabit/second
};

/*
 * Gross bus status. Does not include queue status.
 */
enum class CanBusStatus {
  DOWN,        // Bus is turned off and unavailable until it is
               // installed.
  STOPPED,     // Bus is installed but not running. Must be started
               // before use. Note that the bus comes up stopped.
  ACTIVE,      // Node is participating in bus activity and can
               // transmit (mode permitting) and receive.
  ERROR_HALT,  // Bus has halted due to errors and can neither
               // send nor receive messages. Bus must recover
               // before it can be used.
  RECOVERING,  // Bus is recovering from an ERROR_HALT. If
               // it recovers successfully, it will enter the
               // STOPPED state, and must be restarted before
               // it can participate in bus activity.
  CORRUPT,     // An internal failure has corrupted the API. Recovery
               // is impossible.
};

enum class CanBusNumber {
  BUS_0,
  BUS_1,
};

#endif /* LIBRARIES_CANBUS_SRC_CANENUMERATIONS_H_ */
