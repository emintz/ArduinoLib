/*
 * TextToFlash32.h
 *
 *  Created on: Apr 14, 2025
 *      Author: Eric Mintz
 *
 * Persists text-valued properties to EEPROM.
 *
 * API for persisting values to EEPROM.
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

#ifndef TEXTTOFLASH_H_
#define TEXTTOFLASH_H_

#include "BasePersister.h"

class TextToFlash : public BasePersister {
protected:
  virtual Flash32Status save(
      const char *field_name,
      const char *field_value,
      Flash32Namespace& flash_memory) const override;
public:
  TextToFlash();
  virtual ~TextToFlash();
};

#endif /* TEXTTOFLASH_H_ */
