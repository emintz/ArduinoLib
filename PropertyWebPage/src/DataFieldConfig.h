/*
 * PropertyConfig.h
 *
 *  Created on: Feb 7, 2025
 *      Author: Eric Mintz
 *
 * Property configuration. Contains the property value
 * (when entered) and the property configuration.
 *
 * TODO(emintz): support radio buttons and check boxes.
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

#include "DataFieldFunction.h"
#include "DataTypeCharacteristics.h"
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
  const DataFieldFunction& persister;
  std::map<const std::string, std::string> label_attributes;
  std::map<const std::string, std::string> value_attributes;

  /**
   * Sets the initial tag property values.
   */
  void init_attributes(const char *type);

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

  struct Configuration;

  DataFieldConfig();

  /*
   * Creates a data field configuration from the specified configuration.
   * The configuration contains all required information.
   */
  DataFieldConfig(const Configuration& configuration);

  DataFieldConfig(const DataFieldConfig& copy_me);

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
   * Return: the field name.
   */
  const std::string& get_field_name(void) const {
    return name;
  }

  /*
   * Return: the property identifier.
   */
  const std::string& get_id(void) const {
    return id;
  }

  /*
   * Return: the current value
   */
  const std::string& get_value(void) const {
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
   * Returns: true if the value is valid, false otherwise.
   */
  bool validate_value(void) const;

  struct Configuration {
    const char *id;
    const char *label;
    const char *name;
    const char *initial_value;
    const char *type;
    const PropertyValidator *validator;
    const DataFieldFunction& initializer;
    const DataFieldFunction& persister;
    const std::map<const std::string, std::string> attributes;

    Configuration();

    Configuration(const char* id_and_name, const char *label);

    Configuration(
        const char* id_and_name,
        const char *label,
        const DataTypeCharacteristics& characteristics);

    Configuration& set_type(const char *type_value) {
      type = type_value;
      return *this;
    }
  };
};

#endif /* DATAFIELDCONFIG_H_ */
