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

#include "Arduino.h"

#include <src/FieldLayout.h>

FieldLayout::FieldLayout() :
    ordered_by_id(),
    duplicate_ids(),
    ordered_by_insertion() {
}

FieldLayout::~FieldLayout() {
}

FieldLayout& FieldLayout::add_field(
    const PropertyValidator *validator,
    const char *id,
    const char *label,
    const char *name,
    const char *initial_value,
    const char *type) {
  auto emplacement = ordered_by_id.try_emplace(
      id,   // Key
      validator,
      id,
      label,
      name,
      initial_value,
      type);
  if (emplacement.second) {
    const auto & node = *emplacement.first;
    ordered_by_insertion.push_back(&(node.second));
  } else {
    duplicate_ids.insert(id);
  }
  return *this;
}

FieldLayout& FieldLayout::apply(DataFieldFunction& action) {
  for (auto iter = ordered_by_id.begin(); iter != ordered_by_id.end(); ++iter) {
    action(iter->second);
  }
  return *this;
}
