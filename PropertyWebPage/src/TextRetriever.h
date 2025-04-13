/*
 * TextRetriever.h
 *
 *  Created on: Apr 9, 2025
 *      Author: Eric Mintz
 *
 * Initializes text-valued field configurations from flash memory. Length
 * is limited to 63 characters.
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

#ifndef TEXTRETRIEVER_H_
#define TEXTRETRIEVER_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

class TextRetriever : public DataFieldFunction {
  Flash32BaseNamespace& flash_namespace;
  PersistStatus& errors;
public:
  TextRetriever(
      Flash32BaseNamespace& flash_namespace,
      PersistStatus& errors);
  virtual ~TextRetriever();

  virtual bool operator() (DataFieldConfig& configuration) const override;
};

#endif /* TEXTRETRIEVER_H_ */
