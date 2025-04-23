/*
 * Configurator.h
 *
 *  Created on: Apr 20, 2025
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

#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include "FieldLayout.h"
#include "Flash32.h"

class DataTypeCharacteristics;

class Configurator {
  const char *name;
  FieldLayout layout;
  Flash32Namespace eeprom;

  /**
   * Open eeprom so that it can provide and persist values. This
   * involves installing the NVS drivers and opening the namespace.
   * If the invocation succeeds, the NVS drivers will be loaded and
   * running and the namespace will be open. Otherwise, the drivers
   * will not be loaded and the namespace will be closed.
   *
   * Note that the namespace will be created if it does not exist.
   *
   * Returns: true if the invocation succeeds, false otherwise.
   */
  bool nvs_start(void);

  /*
   * Close the associated non-volitile storage namespace and unload
   * non-volitile namespace drivers.
   */
  void nvs_stop(void);

public:
  /*
   * Create an instance that will access the specified namespace. Note
   * that the constructor does not open the namespace.
   */
  Configurator(const char *name);

  /*
   * Constructor. The configurator must not be active when this is
   * called.
   */
  virtual ~Configurator();

  /**
   * Adds an I/O field to the configuration. Web pages display fields and
   * their values in order of addition.
   *
   * Parameter            Contents
   * -------------------- -----------------------------------------
   * id_and_name          The identifier and name for the data entry
   *                      field (the generated <input> ... </input>
   *                      HTML), which becomes the value of its id
   *                      and name tags.
   * label                The label to display to the left of the
   *                      data entry field.
   * characteristics      The characteristics of the field's data
   *                      type, e.g. int32, int64, text. Prefer
   *                      characteristics provided by the DataTypes
   *                      class.
   *
   */
  Configurator& add_field(
      const char *id_and_name,
      const char *label,
      const DataTypeCharacteristics& characteristics);


  /*
   * Open non-volitile storage and run the web server. Persist input
   * values if the user so requests. Shut everything down when done.
   *
   * If the invocation succeeds, the namespace will be created if it
   * did not exist. Transactionality is supported on a best-efforts
   * basis, meaning that the persisted values *should* remain unchanged
   * if the invocation fails, but this is not guaranteed. If a
   * failing invocation creates a namespace, it should be empty.
   *
   * Returns: true if and only if the operation succeeds.
   */
  bool run(void);
};

#endif /* CONFIGURATOR_H_ */
