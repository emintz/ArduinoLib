/*
 * TextPersister.h
 *
 *  Created on: Apr 11, 2025
 *      Author: Eric Mintz
 *
 * Persists a text (i.e. std::string)-typed value. Note that any string,
 * including the empty string is valid.
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

#ifndef TEXTPERSISTER_H_
#define TEXTPERSISTER_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

class TextPersister : public DataFieldFunction {
  Flash32Namespace& flash_namespace;
  PersistStatus& errors;
public:
  TextPersister(
      Flash32Namespace& flash_namespace,
      PersistStatus& errors);
  virtual ~TextPersister();

  virtual bool operator() (DataFieldConfig& field_config) const override;
};

#endif /* TEXTPERSISTER_H_ */
