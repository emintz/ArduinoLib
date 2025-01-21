/*
 * BlinkStatusAction.h
 *
 *  Created on: Nov 21, 2023
 *      Author: Eric Mintz
 *
 * Copyright (C) 2023 Eric Mintz
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
 * Blinks an LED as directed by an incoming command. Commands
 * arrive on a pull queue that the action uses for timing and for message
 * transport.
 */

#ifndef BLINKSTATUSACTION_H_
#define BLINKSTATUSACTION_H_

#include "Arduino.h"
#include "StatusBlinkCommand.h"
#include "TaskAction.h"

template <class T> class PullQueueT;

class BlinkStatusAction : public TaskAction {
  const uint8_t led_pin;
  PullQueueT<StatusBlinkCommand> *queue;


  /**
   * Polls the queue for a new command while blinking the LED once. An incoming
   * messages cancels the blink immediately upon arrival.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- -------------------------------------------------------
   * on_time_millis    LED on time in milliseconds
   * off_time_millis   LED off time in milliseconds
   */
  bool blink_once(
    uint32_t on_time_millis,
    uint32_t off_time_millis);

  /**
   * Indicates the containing application's status by blinking an LED or by
   * holding it dark.
   *
   * Parameters:
   *
   * Name                                     Contents
   * ---------------------------------------- ----------------------------------
   * on_time_millis                           LED on time in milliseconds
   * off_time_millis                          LED off time in milliseconds
   * wait_time_between_blink_sequence_millis  Time to wait between blink groups
   *                                          in milliseconds
   * status_code                              Number of blinks in a group
   */
  void show_status_code(
      uint32_t on_time_millis,
      uint32_t off_time_millis,
      uint32_t wait_between_blink_sequence_millis,
      uint16_t status_code);

  /**
   * Pause between blink groups
   *
   * Name                                     Contents
   * ---------------------------------------- ----------------------------------
   * wait_time_between_blink_sequence_millis  Time to wait between in
   *                                          milliseconds
   *
   * Returns: true if a message arrived, false otherwise. Returns immediately
   * upon message arrival.
   */
  bool pause_between_blinks(uint32_t wait_between_blink_sequence_millis);

  /**
   * Wait forever for an incoming message
   */
  void wait_for_message(void);

  /**
   * Wait until the specified time elapses or until a message arrives,
   * whichever comes first.
   *
   * Parameters:
   *
   *
   * Name              Contents
   * ----------------- -------------------------------------------------------
   * wait_time_millis  Wait time in milliseconds
   *
   * Returns: true if a message has arrived, false if the specified time
   * elapsed without message arrival.
   */
  bool wait_for_message(uint32_t wait_time_millis);

public:
  /**
   * Configures and creates a BlinkStatusAction
   *
   * Parameters:
   *
   * Name     Contents
   * -------- -----------------------------------------------------------------
   * led_pin  The pin that drives the LED to be blinked
   * queue    A pull queue that provides blink commands
   *
   */
  BlinkStatusAction(
      uint8_t led_pin,
      PullQueueT<StatusBlinkCommand> *queue);
  virtual ~BlinkStatusAction();

  /**
   * Runs the logic that receives and responds to blink commands
   */
  virtual void run(void);
};

#endif /* BLINKSTATUSACTION_H_ */
