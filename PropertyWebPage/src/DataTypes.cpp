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

#include "Int32Retriever.h"
#include "Int32ToFlash.h"

#include "Int64Retriever.h"
#include "Int64ToFlash.h"

#include "TextRetriever.h"
#include "TextToFlash.h"

static std::map<const std::string, std::string> number_attributes =
    {{"title", "Please enter a valid decimal number."}};

DataTypes::DataTypes() :
    int32(
        "number",
        std::make_unique<const Int32ToFlash>(),
        std::make_unique<Int32Retriever>(),
        number_attributes),
    int64(
        "number",
        std::make_unique<const Int64ToFlash>(),
        std::make_unique<Int64Retriever>(),
        number_attributes),
    text(
        "text",
        std::make_unique<const TextToFlash>(),
        std::make_unique<TextRetriever>()) {
}

DataTypes::~DataTypes() {
}
