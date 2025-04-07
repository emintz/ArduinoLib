/*
 * PropertyConfig.h
 *
 *  Created on: Feb 7, 2025
 *      Author: Eric Mintz
 *
 * Property configuration. Contains the property value
 * (when entered) and the property configuration.
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
 *
 * Important precondition: please see as_htmp() method.
 */

#ifndef DATAFIELDCONFIG_H_
#define DATAFIELDCONFIG_H_

#include "PropertyValidator.h"

#include <memory>
#include <map>
#include <string>

class DataFieldConfig {
  const PropertyValidator *validator;
  std::string id;     // Input field identifier, used to link the field label
  std::string label;  // Field label
  std::string name;   // Field name
  std::string value;  // Property value, defaults to "" on construction.
                      // Set to display a default value on the screen.
  std::map<const std::string, std::string> label_attributes;
  std::map<const std::string, std::string> value_attributes;


  /*
   * Formats the label as HTML.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               Indent level, number of spaces to indent
   *
   * Returns: the generated HTML, including a terminating new-line.
   */
  std::string as_label_html(int indent) const;

  /*
   * Formats the input field as HTML. The output DOES NOT include a
   * terminating new-line so the caller can append an error message
   * if needed.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               Indent level, number of spaces to indent
   *
   * Returns: the generated HTML, including a terminating new-line.
   */
  std::string as_input_html(int indent) const;

  /*
   * Formats the field value as a table label field.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               Indent level, number of spaces to indent
   *
   * Returns: the generated HTML, including a terminating new-line.
   */
  std::string as_table_label_html(int indent) const;

  /*
   * Formats the field value as a table value field.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               Indent level, number of spaces to indent
   *
   * Returns: the generated HTML, including a terminating new-line.
   */
  std::string as_value_html(int indent) const;

  /*
   * Produces a blank table field if the value is valid, or an
   * error message if it is invalid
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               Indent level, number of spaces to indent
   *
   * Returns: the generated HTML, including a terminating new-line.
   */
  std::string maybe_error_message(int indent) const;

public:

  DataFieldConfig();

  /*
   * Constructor
   *
   * Creates a data field configuration with the specified input field
   * identifier, input field name, input type, and an empty attribute map.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * validator            Validates the user-entered field value
   * id                   Input field identifier and target (i.e. for
   *                      attribute value) for the associated label
   *                      field. Must be unique in the containing web
   *                      page.
   * label                Field label -- displayed to the right of the
   *                      input field
   * name                 Input field name
   * initial_value        Field value on page startup. Defaults to ""
   * type                 Input value type, defaults to "text"
   */
  DataFieldConfig(
      const PropertyValidator *validator,
      const char *id,
      const char *label,
      const char *name,
      const char *initial_value = "",
      const char *type = "text");

  DataFieldConfig(const DataFieldConfig& copy_me);

  DataFieldConfig& operator=(const DataFieldConfig& assign_me);

  virtual ~DataFieldConfig();

  /*
   * Add an attribute to the field's label tag.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * name                 Attribute name. Note that 'id' and 'for'
   *                      'type', and 'value' are reserved. This
   *                      is not enforced, but changing their
   *                      values will cause undefined behavior.
   * value                Attribute value
   */
  void add_label_attribute(std::string name, std::string value);

  /*
   * Add an attribute to the field's value tag.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * name                 Attribute name. Note that 'id' and 'for'
   *                      'type', and 'value' are reserved. This
   *                      is not enforced, but changing their
   *                      values will cause undefined behavior.
   * value                Attribute value
   */
  void add_value_attribute(std::string name, std::string value);

  /*
   * Format the property as HTML for an input field with the current value
   * filled in. The value is not validated. The returned value will
   * display as a table row.
   *
   * Note: the following style must be in effect for the generated HTML
   *       do display properly.
   *
   *     <style>
   *       form  { display: table;      }
   *       p     { display: table-row;  }
   *       label { display: table-cell; }
   *       input { display: table-cell; }
   *     </style>
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               The number of spaces to indent the generated
   *                      HTML.
   */
  std::string as_input_form_row(int indent) const;

  /*
   * Format the label and value fields for inclusion in a table
   * row. Note that the result is not embedded in table row tags.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               Indent level, number of spaces to indent
   */
  std::string as_label_and_value_fields(int indent) const;

  /*
   * Format the property as HTML for an input field with the current value
   * filled in. Display an error message if the value is invalid. The
   * returned value will display as a table row.
   *
   * Note: the style described above must be in effect.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * indent               The number of spaces to indent the generated
   *                      HTML.
   */
  std::string as_validated_input_form_row(int indent) const;

  /*
   * Return: the property identifier.
   */
  const std::string& get_id() const {
    return id;
  }

  /*
   * Return: the current value
   */
  const std::string& get_value() const {
    return value;
  }

  /**
   * Set the field value to the specified value
   *
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * value                Value to set
   */
  void set_value(const std::string& value) {
    this->value = value;
  }

  /**
   * Validate the current value
   *
   * Returns: true if the value is valid, false otherwise.
   */
  bool validate_value(void) const;
};

#endif /* DATAFIELDCONFIG_H_ */
