/*
 * FieldLayout.cpp
 *
 *  Created on: Mar 16, 2025
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


#include <src/PersistenceAction.h>
#include "DataFieldFunction.h"
#include "FieldLayout.h"
#include "Flash32.h"
#include "PersistStatus.h"

FieldLayout::FieldLayout() :
    ordered_by_id(),
    duplicate_ids(),
    ordered_by_insertion() {
}

FieldLayout::~FieldLayout() {
}

FieldLayout& FieldLayout::add_field(
    const DataFieldConfig::Configuration configuration) {
  auto emplacement = ordered_by_id.try_emplace(
      configuration.id, configuration);
  if (emplacement.second) {
    auto & node = *emplacement.first;
    ordered_by_insertion.push_back(&(node.second));
  } else {
    duplicate_ids.insert(configuration.id);
  }
  return *this;
}

FieldLayout& FieldLayout::apply(DataFieldFunction& action) {
  for (
      auto iter = ordered_by_insertion.begin();
      iter != ordered_by_insertion.end();
      ++iter) {
    action(**iter);
  }
  return *this;
}

FieldLayout& FieldLayout::apply(
    PersistenceAction& action,
    Flash32Namespace& eeprom,
    PersistStatus& errors) {
  if (eeprom.ready()) {
    for(
        auto iter = ordered_by_insertion.begin();
        iter != ordered_by_insertion.end();
        ++iter) {
      action(**iter, eeprom, errors);
    }
  } else {
    errors.append_error("Non-volatile storage is not ready.");
  }
  return *this;
}
