/*
 * LongValidator.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Eric Mintz
 */

#ifndef SIGNEDDECIMALVALIDATOR_H_
#define SIGNEDDECIMALVALIDATOR_H_

#include "PropertyValidator.h"

class SignedDecimalValidator : public PropertyValidator {
public:
  SignedDecimalValidator();
  virtual ~SignedDecimalValidator();

  bool validate(const std::string& value) const override;
};

#endif /* SIGNEDDECIMALVALIDATOR_H_ */
