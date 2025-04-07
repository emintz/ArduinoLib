/*
 * WebPageBundle.cpp
 *
 *  Created on: Mar 31, 2025
 *      Author: Eric Mintz
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

#include "PageBundleHandler.h"

PageBundleHandler::PageBundleHandler(
    FieldLayout& layout,
    std::map<std::string, std::unique_ptr<WebPage>>& web_pages) :
    not_found(layout),
    web_pages(web_pages) {

}

PageBundleHandler::~PageBundleHandler() {
}

WebPage& PageBundleHandler::page(const String& uri) {
  std::string key(uri.c_str());
  return web_pages.contains(key)
      ? *(web_pages[key])
      : not_found;
}

bool PageBundleHandler::canHandle(HTTPMethod method, const String& uri) {
  Serial.print("Old style canHandle invoked for ");
  Serial.println(uri.c_str());
  return true;
}

bool PageBundleHandler::canHandle(
    WebServer& server, HTTPMethod method, const String& uri) {
  Serial.print("canHandle called for ");
  Serial.println(uri.c_str());
  return page(uri).can_handle(server, method);
}

bool PageBundleHandler::handle(
    WebServer& server, HTTPMethod method, const String& uri) {
  Serial.print("handle called for ");
  Serial.println(uri.c_str());
  return page(uri).handle(server, method, uri);
}
