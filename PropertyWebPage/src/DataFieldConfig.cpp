/*
 * PropertyConfig.cpp
 *
 *  Created on: Feb 7, 2025
 *      Author: Eric Mintz
 *
 * Copyright (c) 2025, Eric Mintz
 * All Rights reserved.
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

#include "DataTypeCharacteristics.h"
#include "DataFieldConfig.h"
#include "SetBlankValue.h"
#include "ToFlash32Persister.h"
#include "VacuousDataFieldFunction.h"
#include "VacuousFlash32Persister.h"

static const std::string start_property_value("\"");
static const std::string end_property_value("\" ");
static const std::string for_property_name("for");
static const std::string id_property_name("id");
static const std::string name_property_name("name");
static const std::string type_property_name("type");
static const std::string value_property_name("value");

static SetBlankValue default_initializer;
static VacuousDataFieldFunction default_persister;
static const VacuousFlash32Persister default_flash32_persister;

static std::string make_property(
    const std::string name, const std::string value) {
  std::string property;
  property.append(name)
      .append("=")
      .append(start_property_value)
      .append(value)
      .append(end_property_value);
  return property;
}

DataFieldConfig::DataFieldConfig() :
  id(),
  label(),
  name(),
  value(),
  label_attributes(),
  value_attributes(),
  initializer(default_initializer),
  persister(default_flash32_persister) {
}

DataFieldConfig::DataFieldConfig(
    const DataFieldConfig::Configuration& configuration) :
      id(configuration.id),
      label(configuration.label),
      name(configuration.name),
      value(),
      initializer(configuration.initializer),
      persister(configuration.persister),
      value_attributes(configuration.attributes) {
  init_attributes(configuration.type);
  configuration.initializer(*this);
}

DataFieldConfig::DataFieldConfig(const DataFieldConfig& copy_me) :
  id(copy_me.id),
  label(copy_me.label),
  name(copy_me.name),
  value(copy_me.value),
  label_attributes(copy_me.label_attributes),
  value_attributes(copy_me.value_attributes),
  initializer(copy_me.initializer),
  persister(copy_me.persister) {

}

DataFieldConfig::~DataFieldConfig() {
}

std::string DataFieldConfig::as_input_form_row(int indent) const {
  std::string html;
  html
      .append(indent, ' ')
      .append("<p>\n")
      .append(as_label_html(indent + 2))
      .append(as_input_html(indent + 2))
      .append("\n")
      .append(indent, ' ')
      .append("</p>\n");
  return html;
}

void DataFieldConfig::add_label_attribute(std::string name, std::string value) {
  label_attributes[name] = make_property(name, value);
}

void DataFieldConfig::add_value_attribute(std::string name, std::string value) {
  value_attributes[name] = make_property(name, value);
}

void DataFieldConfig::init_attributes(const char *type) {
  label_attributes[for_property_name] =
      make_property(for_property_name, id);
  label_attributes[id_property_name] =
      make_property(id_property_name, id);

  value_attributes[id_property_name] =
      make_property(id_property_name, id);
  value_attributes[name_property_name] =
      make_property(name_property_name, name);
  value_attributes[type_property_name] =
      make_property(type_property_name, type);

}

std::string DataFieldConfig::as_label_html(int indent) const {
  std::string html(indent, ' ');
  html.append("<label ");
  for (auto it = label_attributes.begin(); it != label_attributes.end(); ++it) {
    html.append(it->second).append(" ");
  }
  html.append(">").append(label).append("</label>\n");
  return html;
}

std::string DataFieldConfig::as_input_html(int indent) const {
  std::string html(indent, ' ');
  html.append("<input ");
  for (auto it = value_attributes.begin(); it != value_attributes.end(); ++it) {
    html.append(it->second).append(" ");
  }
  html
      .append("value=\"")
      .append(value)
      .append("\" >");
  return html;
}

std::string DataFieldConfig::as_value_html(int indent) const {
  std::string html(indent, ' ');
  html.append("<td ");
  for (auto it = value_attributes.begin(); it != value_attributes.end(); ++it) {
    html.append(it->second).append(" ");
  }
  html.append(">").append(value).append("</td>\n");
  return html;
}

std::string DataFieldConfig::as_label_and_value_fields(int indent) const {
  std::string html;
  html
      .append(indent, ' ')
      .append("<tr>\n")
      .append(as_table_label_html(indent + 2))
      .append(as_value_html(indent + 2))
      .append(indent, ' ')
      .append("</tr>\n")
  ;
  return html;
}

std::string DataFieldConfig::as_table_label_html(int indent) const {
  std::string html(indent, ' ');
  html.append("<td ");
  for (auto it = label_attributes.begin(); it != label_attributes.end(); ++it) {
    html.append(it->second).append(" ");
  }
  html.append(">").append(label).append("</td>\n");
  return html;
}

DataFieldConfig::Configuration::Configuration() :
  id(""),
  label(""),
  name(""),
  initial_value(""),
  type("text"),
  initializer(default_initializer),
  persister(default_flash32_persister) {
}

DataFieldConfig::Configuration::Configuration(
    const char *id_and_name,
    const char *label) :
      id(id_and_name),
      label(label),
      name(id_and_name),
      initial_value(""),
      type("text"),
      initializer(default_initializer),
      persister(default_flash32_persister) {
}

DataFieldConfig::Configuration::Configuration(
    const char *id_and_name,
    const char *label,
    const DataTypeCharacteristics& characteristics) :
          id(id_and_name),
          label(label),
          name(id_and_name),
          initial_value(""),
          type(characteristics.type_name().c_str()),
          initializer(characteristics.retriever()),
          persister(characteristics.persister()),
          attributes(characteristics.attributes()) {
}
