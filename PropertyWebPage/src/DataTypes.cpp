/*
 * DataTypes.cpp
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

#include "DataTypes.h"

#include "DataFieldFunction.h"
#include "DataTypeCharacteristics.h"
#include "Flash32.h"
#include "PersistStatus.h"

#include "Int32Retriever.h"
#include "Int32ToFlash.h"

#include "Int64Retriever.h"
#include "Int64ToFlash.h"

#include "TextRetriever.h"
#include "TextToFlash.h"

static std::map<const std::string, std::string> number_attributes =
    {{"title", "Please enter a valid decimal number."}};

DataTypes::DataTypes(
    Flash32Namespace& flash_namespace,
    PersistStatus& errors) :
    int32(
        "number",
        std::make_unique<const Int32ToFlash>(),
        std::make_unique<const Int32Retriever>(flash_namespace, errors),
        number_attributes),
    int64(
        "number",
        std::make_unique<const Int64ToFlash>(),
        std::make_unique<const Int64Retriever>(flash_namespace, errors),
        number_attributes),
    text(
        "text",
        std::make_unique<const TextToFlash>(),
        std::make_unique<const TextRetriever>(flash_namespace, errors)) {
}

DataTypes::~DataTypes() {
}
