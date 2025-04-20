/*
 * WebPageBundle.h
 *
 *  Created on: Mar 31, 2025
 *      Author: Eric Mintz
 *
 * A RequestHandler (an implementer of the WebServer's HTML handling API)
 * that routes requests to a bundle of web pages indexed by path. Pages are
 * indexed by their URL path; requests are routed to the matching page or
 * to a default "path not found" web page. This design presumes that the pages
 * all belong to the same domain.
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

#ifndef WEBPAGEBUNDLE_H_
#define WEBPAGEBUNDLE_H_

//#include "FieldLayout.h"
#include "PageNotFound.h"
#include "WebPage.h"

#include <WebServer.h>

#include <map>

class FieldLayout;

class PageBundleHandler : public RequestHandler {

  PageNotFound not_found;
  std::map<std::string, std::unique_ptr<WebPage>>& web_pages;

  WebPage& page(const String& uri);


public:
  /*
   * Constructs a handler from a field layout and a map of
   * URL path |-> web page. The layout is used to construct
   * the "page not found" handler.
   *
   * TODO(emintz): pass the "page not found" handler explicitly instead
   *               of constructing it in the constructor.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * layout               The fields to be set. These are used to
   *                      construct the "page not found" handler.
   *                      We will pass the not found handler explicitly
   *                      in the next revision.
   * web_pages            URL path |-> web page.
   */
  PageBundleHandler(
      ServerStatus& status,
      FieldLayout& layout,
      std::map<std::string, std::unique_ptr<WebPage>>& web_pages);
  virtual ~PageBundleHandler();

  virtual bool canHandle(HTTPMethod method, const String& uri) override;

  virtual bool canHandle(
      WebServer& server, HTTPMethod method, const String& uri) override;

  virtual bool handle(
      WebServer& server, HTTPMethod method, const String& uri) override;
};

#endif /* WEBPAGEBUNDLE_H_ */
