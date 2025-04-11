/*
 * Flash32.h
 *
 *  Created on: Dec 6, 2023
 *      Author: Eric Mintz
 *
 * Flash Storage-based EEPROM emulator for the ESP32
 *
 * Copyright (C) 2023 Eric Mintz
 * All Rights Reserved
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
 * This library maintains data in the specified flash partitions. In addition
 * to supporting the usual primitive data types, it natively supports struct
 * storage. It does not support untyped blobs. Use a struct instead.
 *
 */

#ifndef FLASH32_H_
#define FLASH32_H_

#include "Arduino.h"

#include "nvs.h"

/**
 * Possible global Flash32 API states.
 */
enum class Flash32MemoryState {
  CLOSED,     // Can be opened. Flash memory must be opened before use
  OPENED,     // Flash memory is open and ready for use.
  CORRUPTED,  // Flash memory cannot be opened because it is corrupted,
              // truncated, or internal structures cannot be allocated.
};

/**
 * Flass32 access status.
 */
enum class Flash32Status {
  OK,   // Operation succeeded
  NOT_FOUND,    // Key not found
  NO_ROOM,      // Inadequate memory for value (read)
  INVALID_KEY,  // Null or invalid key
  CLOSED,       // Namespace is closed
  NOT_STARTED,  // Flash32.begin() not invoked.
  FAILED,       // Operation failed, internal or unknown error, etc.
};

class Flash32BaseNamespace;

/**
 * Manages the global flash memory state. Use Flash32, the library's singleton
 * instance instead of creating an instance of this class.
 */
class HardwareFlash32 final {
  Flash32MemoryState flash32_state;

  void init_fields(void);
public:

  HardwareFlash32(void);

  ~HardwareFlash32();


  /**
   * Opens non-volatile storage and retrieve usage statistics if and only if
   * it is currently closed. Return true if and only if non-volatile storage is
   * currently open or has been opened successfully. Note that the NVS state
   * will be set to OPENED when the method returns true and to CORRUPTED
   * when the method returns false. Note that this method is not thread-safe.
   */
  bool begin(void);

  /**
   * Closes non-volatile storage if it is open. Does nothing if storage is
   * closed. Note that the NVS state will be set to CLOSED when the call
   * returns true and to CORRUPTED when it returns false. This method is
   * not thread-safe.
   */
  bool end(void);

  /**
   * Return true if and only if non-volatile storage is open and ready for use.
   */
  inline bool ready(void) {
    return Flash32MemoryState::OPENED == flash32_state;
  }

  /**
   * Return the current state. Prefer ready() where possible.
   */
  inline Flash32MemoryState state(void) {
    return flash32_state;
  }

  /**
   * Retrieves the non-volatile storage use statistics. Note that the results
   * are valid if and only if ready() returns true.
   *
   * Parameters
   *
   * Name
   * -------------- ----------------------------------------------------
   * nvs_stats      Contains use statistics for the default non-volitile
   *                storage partition after a successful invocation. Contents
   *                are unspecified on failure.
   *
   * Returns true when retrieval succeeds and false otherwise.
   */
  bool statistics(nvs_stats_t *nvs_stats);
};

class Flash32BaseNamespace {
  friend class Flash32Iterator;

  const char *name;
  nvs_handle_t h_namespace;
  Flash32MemoryState namespace_state;

  /**
   * Reads a sequence of bytes (a,k.a, a "blob") from flash memory.
   *
   * Parameters:
   *
   * Name              Contents
   * ----------------- ------------------------------------------------------
   * key               The key for the stored value
   * value             Holds the retrieved blob
   * length            Length of the provided blob storage in bytes
   * retrieved_length  Number of bytes actually retrieved
   *
   * Returns: the retrieval status
   *
   * When an error occurs, retrieved_length is not changed and *value is
   * unspecified.
   *
   */
  Flash32Status get_blob(
      const char *key,
      void *value,
      size_t length,
      size_t *retrieved_length);

protected:

  /**
   * Forbids subclasses from altering the namespace handle.
   */
  inline nvs_handle_t handle(void) {
    return h_namespace;
  }

  inline Flash32BaseNamespace(
      const char *name) :
        name(name),
        h_namespace(NULL),
        namespace_state(Flash32MemoryState::CLOSED) {
  }

  virtual ~Flash32BaseNamespace() {}

  /**
   * Opens the namespace.
   *
   * Parameters:
   *
   * Name       Contents
   * ---------- --------------------------------------------------------------
   * open_mode  How to open the namespace: read-only or read-write.
   */
  Flash32Status open_namespace(nvs_open_mode_t open_mode);

public:

  /**
   * Closes the namespace. Return s true on success and false on failure. If
   * the namespace is closed, the method does nothing and returns true. If
   * the namespace is corrupt, the method does nothing and returns false. A
   * successfully closed namespace can be reopened.
   */
  Flash32Status close(void);

  /*
   * Retrieves the number of key, value pairs in the namespace. Note that
   * the namespace must be open for the function to succeeds.
   *
   * Parameters:
   *
   * +=============+======================================================+
   * | Name        | Contents                                             |
   * +=============+======================================================+
   * | entry_count | Receives the number of entries in the namespace.     |
   * |             | Cannot be NULL. Contents unspecified on failure.     |
   * +-------------+------------------------------------------------------+
   *
   * Returns: true if and only if retrieval succeeded.
   */
  bool entries(size_t *entry_count);

  /**
   * The following methods retrieve numeric values from flash memory. They
   * behave identically except that they retrieve different value types.
   * Note that all types are integral.
   *
   * Parameters:
   *
   * Name   Contents
   * ------ ---------------------------------------------------------------
   * key    Key
   * value  Receives the retrieved value
   *
   * Returns: the results of the operation.
   */
  Flash32Status get_int8(const char *key, int8_t *value);

  Flash32Status get_uint8(const char *key, uint8_t *value);

  Flash32Status get_int16(const char *key, int16_t *value);

  Flash32Status get_uint16(const char *key, uint16_t *value);

  Flash32Status get_int32(const char *key, int32_t *value);

  Flash32Status get_uint32(const char *key, uint32_t *value);

  Flash32Status get_int64(const char *key, int64_t *value);

  Flash32Status get_uint64(const char *key, uint64_t *value);

  inline const char *get_name(void) const {
    return name;
  }

  /**
   * Retrieves a NULL-terminated string from flash memory
   *
   * Parameters:
   *
   * Name    Value
   * ------- -----------------------------------------------------------------
   * key     Key
   * value   Receives the retrieved value
   * buf_len Length of the provided value storage
   * out_len Receives the number of bytes retrieved
   *
   * Returns: the operation status
   */
  Flash32Status get_str(
      const char *key,
      char *value,
      size_t buf_len,
      size_t *out_len);

  /**
   * Retrieves a struct of type S from flash memory. S must be a classic,
   * self-contained, C style structure having no pointer-valued fields.
   *
   * Parameters:
   *
   * Name            Contents
   * -----           --------------------------------------------------------
   * Key             The key
   * value           The struct to be stored
   * bytes_retrieved The number of bytes retrieved from flash
   *
   * Returns: the operation status
   */
  template <class S> inline Flash32Status get_struct(
      const char *key, S *value, size_t *bytes_retrieved) {
    return get_blob(key, value, sizeof(S), bytes_retrieved);
  }

  /**
   * Return true when the namespace is ready, and false otherwise.
   */
  inline bool ready() {
    return Flash32MemoryState::OPENED == namespace_state;
  }

  /**
   * Return the current state. Prefer ready() where possible.
   */
  inline Flash32MemoryState state(void) {
    return namespace_state;
  }
};

/**
 * A read-only namespace in flash memory
 */
class Flash32ReadOnlyNamespace final : public Flash32BaseNamespace {
public:
  inline Flash32ReadOnlyNamespace(const char *name) :
      Flash32BaseNamespace(name) {
  }

  virtual ~Flash32ReadOnlyNamespace();

  Flash32Status open(void);
};

/**
 * A named read-write namespace in flash memory
 */
class Flash32Namespace final : public Flash32BaseNamespace {
  friend class Flash32Iterator;

  const bool autocommit;

  Flash32Status finish_mutation(esp_err_t status);

  /**
   * Writes a sequence of bytes (a.k.a.a "blob") to flash memory
   *
   * Parameters
   *
   * Name    Contents
   * ------- -----------------------------------------------------------------
   * key     Key for retrieving the stored value
   * value   Pointer to the blob value to store
   * length  The length of the blob in bytes
   *
   * The call will fail if the target namespace was opened for read only.
   * To ensure that the value is actually set, be sure to invoke commit()
   * after set_blob() returns Flash32Status::OK.
   */
  Flash32Status set_blob(
      const char *key,
      void *value,
      size_t length);

public:
  /**
   * Configures a new Flash32Namespace instance, which will be in the CLOSED
   * state. Be sure to invoke Flash32.begin() before attempting to
   * use the namespace.
   */
  inline Flash32Namespace(
      const char *name,
      bool autocommit = true) :
        Flash32BaseNamespace(name),
        autocommit(autocommit) {
  }

  /**
   * Destructor, which closes the namespace if it is open. Best practice is to
   * close namespaces by invoking close().
   */
  virtual ~Flash32Namespace();

  /**
   * Open the namespace. Return true if and only if the namespace was opened
   * successfully. The namespace will be opened in read/write mode, which
   * means it will be created if it does not already exist. The state
   * will be set to OPENED if the call succeeds and to CORRUPTED otherwise.
   * Note that the call will fail if the containing NonVolatileStorage is
   * not open, and that there is no recovery from a failed call. This method
   * is idempotant and not guaranteed to be thread-safe.
   */
  Flash32Status open(void);

  /**
   * Writes all pending changes to flash memory
   */
  Flash32Status commit(void);

  /**
   * Erases a key, value pair
   *
   * Parameters:
   *
   * Name Contents
   * ---- ---------------------------------------------
   * key  Key of the target key, value pair
   *
   * Returns: the results of the operation.
   */
  Flash32Status erase(const char *key);

  /**
   * Erase all entries in the namespace. Be sure to commit after invoking this
   * method.
   *
   * Returns: the results of the operation.
   *
   * When you call this, make sure you mean it!
   */
  Flash32Status erase_all(void);

  // Primitive type accessors. Note that setters will fail if the namespace
  // was opened for read only. All set operations behave identically, as do
  // all get methods. Therefore, only the first are documented.

  /**
   * The following methods store integral values to flash memory. They behave
   * identically except for the type of the stored value.
   *
   * Parameters:
   *
   * Name   Contents
   * ------ ---------------------------------------------------------------
   * key    Key
   * value  The value to store
   *
   * Returns: the results of the operation.
   */
  Flash32Status set_int8(const char *key, int8_t value);

  Flash32Status set_uint8(const char *key, uint8_t value);

  Flash32Status set_int16(const char *key, int16_t value);

  Flash32Status set_uint16(const char *key, uint16_t value);

  Flash32Status set_int32(const char *key, int32_t value);

  Flash32Status set_uint32(const char *key, uint32_t value);

  Flash32Status set_int64(const char *key, int64_t value);

  Flash32Status set_uint64(const char *key, uint64_t value);

  /**
   * Stores a NULL-terminated string in flash memory
   *
   * Parameters:
   *
   * Name  Contents
   * ----- -------------------------------------------------------------------
   * key   Key
   * value NULL terminated string
   *
   * Returns: the operation status
   *
   * The function determines the length of the input string
   */
  Flash32Status set_str(const char *key, const char *value);

  /**
   * Stores a struct of type S in flash memory. S must be a classic,
   * self-contained, C style structure having no pointer-valued fields.
   *
   * Parameters:
   *
   * Name  Contents
   * ----- --------------------------------------------------------------------
   * Key   The key
   * value The struct to be stored
   *
   * Returns: the operation status
   */
  template<class S> inline Flash32Status set_struct(
      const char *key, S *value) {
    return set_blob(key, value, sizeof(S));
  }
};

/**
 * An iterator that traverses a namespace, one entry at a time. When an
 * entry is found, the iterator can provide its key and value type, with
 * structs being blobs.
 */
class Flash32Iterator final {

  enum class IteratorState {
    CLOSED,
    RUNNING,
    FINISHED,
    DELETED,
  };

  const Flash32BaseNamespace &flash32_namespace;
  nvs_type_t data_type;
  IteratorState state;
  nvs_iterator_t iterator;
  nvs_entry_info_t entry_info;

  /**
   * Advance to the next entry, if found. Invoke only when the iterator has
   * been opened successfully.
   *
   * Returns: true if the requested entry exists, false otherwise
   */
  bool advance(void);

  /**
   * Loads the current entry's key and value into entry_info. The
   * iterator must be valid when this method is invoked.
   *
   * Returns: true if the load succeeds, false otherwise.
   */
  bool load(void);

  /**
   * Opens the iterator and positions it at the first entry, if it exists.
   *
   * Returns: true if the iterator was opened successfully and an entry
   * exists, false otherwise. KInvoke this only when the iterator is closed.
   */
  bool start(void);

  /**
   * Closes the iterator and release all associated resources.
   */
  void stop(void);

public:

  Flash32Iterator(
      const Flash32BaseNamespace &flash32_namespace,
      nvs_type_t data_type = NVS_TYPE_ANY);
  ~Flash32Iterator();

  /**
   * Advances to the next key, value pair, if any.
   *
   * Returns: true if a value is available, false if not.
   */
  bool next(void);

  /**
   * Returns the current entry's key, with the value unspecified if the
   * iterator does not have a current entry.
   */
  inline const char *key(void) {
    return entry_info.key;
  }

  /**
   * Returns the current entry's data type, with the value unspecified if
   * the iterator does not have a current entry.
   */
  inline nvs_type_t type(void) {
    return entry_info.type;
  }
};

extern HardwareFlash32 Flash32;

#endif /* FLASH32_H_ */
