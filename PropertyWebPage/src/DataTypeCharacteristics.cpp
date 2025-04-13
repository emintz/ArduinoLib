/*
 * DataTypeCharacteristics.cpp
 *
 *  Created on: Apr 11, 2025
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

#include "DataTypeCharacteristics.h"

DataTypeCharacteristics::DataTypeCharacteristics(
    const char *name,
    std::unique_ptr<const DataFieldFunction> persister,
    std::unique_ptr<const DataFieldFunction> retriever,
    std::unique_ptr<const PropertyValidator> validator,
    const std::map<const std::string, std::string>& attributes) :
        data_type_name(name),
        field_persister(std::move(persister)),
        field_retriever(std::move(retriever)),
        field_validator(std::move(validator)),
        field_attributes(attributes) {
}

DataTypeCharacteristics::~DataTypeCharacteristics() {
}

