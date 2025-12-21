/*
 * Configurator.cpp
 *
 *  Created on: Apr 20, 2025
 *      Author: Eric Mintz
 *
 * Configures an system, storing field values in EEPROM. The user
 * specifies the target EEPROM namespace.
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

#include "Configurator.h"

#include "DataFieldConfig.h"
#include "ConfigurationCancelledPage.h"
#include "ConfirmationPage.h"
#include "CurrentTaskBlocker.h"
#include "DataEntryPage.h"
#include "DataTypes.h"
#include "EmptyPage.h"
#include "PageBundleHandler.h"
#include "RedirectToHome.h"
#include "SaveConfiguration.h"
#include "ServerStatus.h"
#include "TaskWithActionH.h"
#include "WebPage.h"
#include "WebServer.h"
#include "WebServerAction.h"

#include <map>
#include <memory>

Configurator::Configurator(
    const char *name) :
        name(name),
        layout(),
        eeprom(name, false) {
}

Configurator::~Configurator() {
}

bool Configurator::nvs_start(void) {
  bool status = Flash32.begin();

  if (status) {
    status = eeprom.open() == Flash32Status::OK;
    if (!status) {
      Flash32.end();
    }
  }
  return status;
}

void Configurator::nvs_stop() {
  Serial.println("Stopping eeprom.");
  eeprom.close();
  Flash32.end();
  Serial.println("eeprom stopped.");
}

Configurator& Configurator::add_field(
    const char *id_and_name,
    const char *label,
    const DataTypeCharacteristics& characteristics) {
    layout.add_field(DataFieldConfig::Configuration(
        id_and_name,
        label,
        characteristics));
  return *this;
}

bool Configurator::run(
    WebServer& web_server,
    const char *landing_page) {
  Serial.printf("Configurator::run %d landing page: %s\n",
      __LINE__, landing_page);
  bool status = false;
  if (nvs_start()) {
    ServerStatus server_status;
    std::map<std::string, std::unique_ptr<WebPage>> web_pages;
    web_pages[landing_page] = std::make_unique<DataEntryPage>(
        server_status,
        eeprom,
        layout,
        "Configure Practice Field Settings");
    web_pages["/save_changes"] = std::make_unique<ConfirmationPage>(
        server_status,
        layout,
        "Confirm configuration");
    web_pages["/favicon.ico"] = std::make_unique<EmptyPage>(
        server_status,
        layout);
    web_pages["/redirect-to-home"] = std::make_unique<RedirectToHome>(
        server_status,
        layout,
        landing_page);
    web_pages["/configuration-unchanged"] =
        std::make_unique<ConfigurationCancelledPage>(
            server_status, layout);
    web_pages["/save-configuration"] = std::make_unique<SaveConfiguration>(
        server_status, eeprom, layout);

    auto blocker = std::make_unique<CurrentTaskBlocker>();
    auto handler =
        std::make_unique<PageBundleHandler>(server_status, layout, web_pages);
    web_server.addHandler(handler.get());
    auto action = std::make_unique<WebServerAction>(
        web_server, server_status, *(blocker.get()));
    TaskWithActionH web_server_refresh_task(
        "Web server refresh",
        12,
        action.get(),
        8 * 1024);
    web_server.begin(80);
    web_server_refresh_task.start();
    Serial.println("Web server refresh running, waiting for notification.");
    blocker->wait();
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.println("Stopping web server");
    web_server.stop();
    Serial.println("Web server stopped.");
    web_server_refresh_task.stop();
    Serial.println("Web server refresh task stopped.");
    nvs_stop();
    Serial.println(
        "EEPROM management stopped. "
        "Waiting for the system to shut down.");
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.println("Configuration successful, returning from configurator.");
    status = true;
  }

  vTaskDelay(2000);
  return status;
}
