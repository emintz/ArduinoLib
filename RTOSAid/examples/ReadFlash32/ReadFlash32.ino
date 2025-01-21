#include "Arduino.h"

/**
 *  Created on: Jan 20, 2025
 *      Author: Eric Mintz
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
 *
 * Reads a blink configuration from the Flash32Example flash namespace and
 * blinks the built-in LED accordingly.
 *
 * Be sure to initialize flash memory with WriteFlash32 before attempting
 * to run this sketch.
 */
#include "BlinkAction.h"
#include "Flash32.h"
#include "TaskWithActionH.h"
#include <map>
#include <string>

#define FLASH_MEMORY_NAMESPACE "BlinkAction"
#define TASK_NAME "Flash32Example"
#define BUILTIN_LED_PIN 2
#define GROUP_COUNT 5
#define ON_TIME 50
#define OFF_TIME 75
#define WAIT_TIME 500

static std::map<Flash32Status, std::string> status_to_name;
static BlinkAction *action;
static TaskWithActionH *task;

void populate_status_map(void) {
  status_to_name[Flash32Status::OK] = "OK";
  status_to_name[Flash32Status::NOT_FOUND] = "Not Found";
  status_to_name[Flash32Status::NO_ROOM] = "Flash Memory Full";
  status_to_name[Flash32Status::INVALID_KEY] = "Invalid Key";
  status_to_name[Flash32Status::CLOSED] = "Namespace Closed";
  status_to_name[Flash32Status::NOT_STARTED] = "Flash Memory API Not Started";
  status_to_name[Flash32Status::FAILED] = "Unknown Failure Type";
}

void halt_and_catch_fire(const char *error_message) {
  Serial.println(error_message);
  while (true) {
    vTaskDelay(portMAX_DELAY);
  }
}

void check_flash32_status(Flash32Status status, const char *message) {
  if (status != Flash32Status::OK) {
    Serial.printf("Operation failed with status %s\n", status_to_name[status].c_str());
    halt_and_catch_fire(message);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf(
      "Read from flash memory example compiled on %s at %s.\n",
      __DATE__,
      __TIME__);

  populate_status_map();

  // Start the flash memory API.
  if (!Flash32.begin()) {
    halt_and_catch_fire("Flash memory initialization failed.");
  }

  Flash32ReadOnlyNamespace blink_configuration(FLASH_MEMORY_NAMESPACE);

  check_flash32_status(
     blink_configuration.open(),
     "Flash memory namespace open failed");

  char task_name[128];
  memset(task_name, 0, sizeof(task_name));
  size_t task_name_len = 0;
  check_flash32_status(
      blink_configuration.get_str(
          "TaskName",
          task_name,
          sizeof(task_name),
          &task_name_len),
      "Error reading the task name");

  uint16_t gpio_pin = 0;
  check_flash32_status(
      blink_configuration.get_uint16("BuiltinLedPin", &gpio_pin),
      "Error reading built in LED pin");

  uint16_t group_count = 0;
  check_flash32_status(
      blink_configuration.get_uint16("GroupCount", &group_count),
      "Error reading the group count");

  uint16_t on_time = 0;
  check_flash32_status(
      blink_configuration.get_uint16("OnTime", &on_time),
      "Error reading the on time");

  uint16_t off_time = 0;
  check_flash32_status(
      blink_configuration.get_uint16("OffTime", &off_time),
      "Error reading off time");

  uint16_t wait_time = 0;
  check_flash32_status(
      blink_configuration.get_uint16("WaitTime", &wait_time),
      "Error reading wait time");

  check_flash32_status(
      blink_configuration.close(),
      "Flash memory namespace closed failed");

  if (!Flash32.end()) {
    halt_and_catch_fire("Flash memory shutdown failed.");
  }

  action = new BlinkAction(
      gpio_pin,
      group_count,
      on_time,
      off_time,
      wait_time);

  task = new TaskWithActionH(
      task_name,
      2,
      action,
      2048);
  task->start();

  Serial.println("Blink configured and started.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
