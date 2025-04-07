/*
 * FieldLayout.h
 *
 *  Created on: Mar 16, 2025
 *      Author: Eric Mintz
 *
 * The layout of fields on a web page. Having this class saves memory by
 * allowing multiple web pages to use the same set of fields.
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

#ifndef FIELDLAYOUT_H_
#define FIELDLAYOUT_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"

#include <map>
#include <set>
#include <vector>

/*
 * The FieldLayout class maintains DataFieldConfig instances in two containers:
 * a vector ordered by insertion, and a map indexed by identifier. The
 * vector orders the input fields on the page, and the map supports
 * setting field values by id.
 *
 * The map owns the created field configuration instances. Since they receive
 * field values so they must be mutable. The references in the vector, which
 * are used to generate the web page, are only read, so they are const.
 */
class FieldLayout {
  std::map<const std::string, DataFieldConfig> ordered_by_id;
  std::set<std::string> duplicate_ids;
  std::vector<const DataFieldConfig *> ordered_by_insertion;

public:
  /*
   * Constructs an instance containing an empty field property map and
   * vector. Please see above for property ownership semantics.
   */
  FieldLayout();

  virtual ~FieldLayout();

  /*
   * Creates a DataFieldConfig from the specified values and adds it to the
   * map and vector. If id is a duplicate, it is added to the duplicate
   * identifier set. Having duplicate identifiers is a severe error.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * validator            Validates the user-entered field value
   * id                   Input field identifier and target (i.e. for
   *                      attribute value) for the associated label
   *                      field. Must be unique in the layout, as duplicates
   *                      are silently rejected.
   * label                Field label -- displayed to the right of the
   *                      input field
   * name                 Input field name
   * initial_value        Field value on page startup. Defaults to ""
   * type                 Input value type, defaults to "text"
   *
   * Returns: *this to support invocation chaining.
   */
  FieldLayout& add_field(
      const PropertyValidator *validator,
      const char *id,
      const char *label,
      const char *name,
      const char *initial_value = "",
      const char *type = "text");


  /*
   * Applies the specified action to every field configuration in the field
   * vector. Application is ordered by field ID..
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * action               A function to invoke on every field
   *                      configuration. The action can perform any
   *                      operation INCLUDING modifying the configuration.
   *
   * Return: a reference to this FieldLayout to support chaining.
   */
  FieldLayout& apply(DataFieldFunction& action);

  /*
   * Returns: true if and only if the map contains an entry a DataField
   *          indexed by the specified key. Included for the sake of
   *          completeness.
   */
  bool contains(const std::string key) {
    return ordered_by_id.contains(key);
  }

  /**
   * Returns true if and only if the configuration is valid. A
   * configuration is valid if and only if it does not contain
   * duplicate field identifiers.
   */
  bool valid(void) {
    return duplicate_ids.empty();
  }

  /**
   * Return all duplicate identifiers.
   */
  const std::set<std::string>& duplicate_identifiers() {
    return duplicate_ids;
  }
};

#endif /* FIELDLAYOUT_H_ */
