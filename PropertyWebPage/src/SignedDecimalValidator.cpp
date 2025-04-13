/*
 * LongValidator.cpp
 *
 *  Created on: Apr 12, 2025
 *      Author: Eric Mintz
 */

#include <src/SignedDecimalValidator.h>
#include <regex>
#include <string>

SignedDecimalValidator::SignedDecimalValidator() :
    PropertyValidator("not a signed decimal integer.") {

}

SignedDecimalValidator::~SignedDecimalValidator() {
}

bool SignedDecimalValidator::validate(const std::string& value) const {
  std::regex decimal_integer_pattern("[-+]?[0 ..9]+");
  return std::regex_match(value, decimal_integer_pattern);
}
