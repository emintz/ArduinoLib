/*
 * Int32Retriever.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Eric Mintz
 *
 * Retrieves an int32_t value from EEProm, formats it as a decimal string,
 * and sets the resulting value in a field configuration.
 */

#ifndef INT32RETRIEVER_H_
#define INT32RETRIEVER_H_

#include "DataFieldConfig.h"
#include "DataFieldFunction.h"
#include "Flash32.h"
#include "PersistStatus.h"

class Int32Retriever : public DataFieldFunction {
  Flash32Namespace& flash_namespace;
  PersistStatus& errors;
public:
  Int32Retriever(
      Flash32Namespace& flash_namespace,
      PersistStatus& errors);
  virtual ~Int32Retriever();

  virtual bool operator() (DataFieldConfig& field_config) override;
};

#endif /* INT32RETRIEVER_H_ */
