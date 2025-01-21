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
 * Creates a BlinkAction namespace in flash memory and populates it with a BlinkAction
 * configuration. When the sketch finishes, the namespace will contain:
 *
 * +--------------+-----------------+-----------------------------------------+
 * | Key          | Value           | Use                                     |
 * +--------------+-----------------+-----------------------------------------+
 * | TaskName     | Flash32Example  | LED blink task name                     |
 * +--------------+-----------------+-----------------------------------------+
 * | LedPin       | BUILTIN_LED_PIN | GPIO pin connected to the built-in LED. |
 * |              |                 | See BUILTIN_LED_PIN below.              |
 * +--------------+-----------------+-----------------------------------------+
 * | GroupCount   | 5               | Size of the flash group                 |
 * +--------------+-----------------+-----------------------------------------+
 * | OnTime       | 50              | LED on time in milliseconds             |
 * +--------------+-----------------+-----------------------------------------+
 * | OffTime      | 75              | LED off time in milliseconds            |
 * +--------------+-----------------+-----------------------------------------+
 * | WaitTime     | 500             | Inter-group wait time in milliseconds   |
 * +--------------+-----------------+-----------------------------------------+
 *
 * Alter the value of BUILTIN_LED_PIN to match your configuration, if necessary.
 * Otherwise, no special configuration is required.
 */

#include "Flash32.h"
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
    Serial.printf("Write failed with status %s\n", status_to_name[status]);
    halt_and_catch_fire(message);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf(
      "Write to flash memory example compiled on %s at %s.\n",
      __DATE__,
      __TIME__);

  populate_status_map();

  // Start the flash memory API.
  if (!Flash32.begin()) {
    halt_and_catch_fire("Flash memory initialization failed.");
  }

  Flash32Namespace blink_configuration(FLASH_MEMORY_NAMESPACE);

  check_flash32_status(
     blink_configuration.open(),
     "Flash memory namespace open failed");

  check_flash32_status(
      blink_configuration.set_str("TaskName", TASK_NAME),
      "Error setting task name");
  check_flash32_status(
      blink_configuration.set_uint16("BuiltinLedPin", BUILTIN_LED_PIN),
      "Setting the built-in LED pin number");
  check_flash32_status(
      blink_configuration.set_uint16("GroupCount", GROUP_COUNT),
      "Error setting the group count");
  check_flash32_status(
      blink_configuration.set_uint16("OnTime", ON_TIME),
      "Error setting the LED on time");
  check_flash32_status(
      blink_configuration.set_uint16("OffTime" ,OFF_TIME),
      "Error setting the LED off time");
  check_flash32_status(
      blink_configuration.set_uint16("WaitTime" ,WAIT_TIME),
      "Error setting the wait time");

  check_flash32_status(
      blink_configuration.close(),
      "Flash memory namespace closed failed");

  // Stop the flash memory API.
  if (!Flash32.end()) {
    halt_and_catch_fire("Flash memory shutdown failed.");
  }

  Serial.println("Configuration written to flash memory.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
