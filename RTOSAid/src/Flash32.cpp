/*
 * Flash32.cpp
 *
 *  Created on: Dec 6, 2023
 *      Author: Eric Mintz
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
 */

#include "Flash32.h"

#import <cstring>

#include "nvs_flash.h"

HardwareFlash32 Flash32;

//----------------------------------------------------------------------------
// HardwareFlash32
//----------------------------------------------------------------------------

void HardwareFlash32::init_fields(void) {
  flash32_state = Flash32MemoryState::CLOSED;
}

HardwareFlash32::HardwareFlash32(void) {
  init_fields();
}

HardwareFlash32::~HardwareFlash32() {
  end();
}

bool HardwareFlash32::begin(void) {
  bool result = true;
  switch (flash32_state) {
  case Flash32MemoryState::CLOSED:
    if (ESP_OK == nvs_flash_init()) {
      flash32_state = Flash32MemoryState::OPENED;
    } else {
      flash32_state = Flash32MemoryState::CORRUPTED;
    }
    break;
  case Flash32MemoryState::OPENED:
    // Nothing to do.
    break;

  case Flash32MemoryState::CORRUPTED:
    result = false;
    break;
  }

  return result;
}

bool HardwareFlash32::end(void) {
  bool result = true;

  switch (flash32_state) {
  case Flash32MemoryState::CLOSED:
    break;
  case Flash32MemoryState::OPENED:
    if (ESP_OK == nvs_flash_deinit()) {
      init_fields();
    }
    else {
      flash32_state = Flash32MemoryState::CORRUPTED;
      result = false;
    }
    break;
  case Flash32MemoryState::CORRUPTED:
    result = false;
    break;
  }

  return result;
}

bool HardwareFlash32::statistics(nvs_stats_t *stats) {
  return ESP_OK == nvs_get_stats(NVS_DEFAULT_PART_NAME, stats);
}

static Flash32Status to_flash32_status(esp_err_t error_code) {
  switch (error_code) {
    case ESP_OK:
      return Flash32Status::OK;
    case ESP_ERR_NVS_NOT_FOUND:
      return Flash32Status::NOT_FOUND;
    case ESP_ERR_NVS_INVALID_LENGTH:
      return Flash32Status::NO_ROOM;
    case ESP_ERR_NVS_INVALID_NAME:
      return Flash32Status::INVALID_KEY;
    case ESP_ERR_NVS_INVALID_HANDLE:
      return Flash32Status::CLOSED;
    case ESP_FAIL:
    default:
      return Flash32Status::FAILED;
  }
}

//----------------------------------------------------------------------------
// Flash32BaseNamespace
//----------------------------------------------------------------------------

Flash32Status Flash32BaseNamespace::get_blob(
    const char *key,
    void *value,
    size_t length,
    size_t *retrieved_length) {
  size_t buffer_length = length;
  Flash32Status result =
      to_flash32_status(
          nvs_get_blob(
              h_namespace,
              key,
              value,
              &buffer_length));
  if (Flash32Status::OK == result) {
    *retrieved_length = buffer_length;
  }
  return result;
}


Flash32Status Flash32BaseNamespace::open_namespace(nvs_open_mode_t open_mode) {
  Flash32Status result =
      Flash32.ready()
          ? Flash32Status::OK
          : Flash32Status::NOT_STARTED;

  if (result == Flash32Status::OK) {
    switch (namespace_state) {
    case Flash32MemoryState::CLOSED:
      result = to_flash32_status(
          nvs_open(
              name,
              open_mode,
              &h_namespace));
        namespace_state = Flash32Status::OK == result
            ? Flash32MemoryState::OPENED
            : Flash32MemoryState::CORRUPTED;
      break;

    case Flash32MemoryState::OPENED:
      // Nothing to do.
      break;

    case Flash32MemoryState::CORRUPTED:
      result = Flash32Status::FAILED;
      break;
    }
  }

  return result;
}

Flash32Status Flash32BaseNamespace::close(void) {
  Flash32Status result = Flash32.ready()
      ? Flash32Status::OK
      : Flash32Status::NOT_STARTED;
  if (Flash32Status::OK == result) {
    switch (namespace_state) {
    case Flash32MemoryState::CLOSED:
      // Nothing to do.
      break;

    case Flash32MemoryState::OPENED:
      nvs_close(h_namespace);
      h_namespace = NULL;
      namespace_state = Flash32MemoryState::CLOSED;
      break;
    case Flash32MemoryState::CORRUPTED:
      result = Flash32Status::FAILED;
      break;
    }
  }
  return result;
}

Flash32Status Flash32BaseNamespace::get_int8(const char* key, int8_t *value) {
  return to_flash32_status(nvs_get_i8(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_uint8(const char *key, uint8_t *value) {
  return to_flash32_status(nvs_get_u8(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_int16(const char* key, int16_t *value) {
  return to_flash32_status(nvs_get_i16(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_uint16(const char *key, uint16_t *value) {
  return to_flash32_status(nvs_get_u16(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_int32(const char* key, int32_t *value) {
  return to_flash32_status(nvs_get_i32(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_uint32(const char *key, uint32_t *value) {
  return to_flash32_status(nvs_get_u32(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_int64(const char* key, int64_t *value) {
  return to_flash32_status(nvs_get_i64(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_uint64(const char *key, uint64_t *value) {
  return to_flash32_status(nvs_get_u64(h_namespace, key, value));
}

Flash32Status Flash32BaseNamespace::get_str(
    const char *key,
    char *value,
    size_t buf_len,
    size_t *out_len) {
  size_t length = buf_len;
  Flash32Status status =
      to_flash32_status(nvs_get_str(handle(), key, value, &length));
  if (Flash32Status::OK == status) {
    *out_len = length;
  }
  return status;
}

//----------------------------------------------------------------------------
// Flash32ReadOnlyNamespace
//----------------------------------------------------------------------------

Flash32ReadOnlyNamespace::~Flash32ReadOnlyNamespace() {
}

//----------------------------------------------------------------------------
// Flash32Namespace
//----------------------------------------------------------------------------

Flash32Namespace::~Flash32Namespace() {
  close();
}

Flash32Status Flash32ReadOnlyNamespace::open(void) {
  return open_namespace(NVS_READONLY);
}

Flash32Status Flash32Namespace::finish_mutation(esp_err_t status) {
  if (status == ESP_OK && autocommit) {
    status = nvs_commit(handle());
  }
  return to_flash32_status(status);
}

Flash32Status Flash32Namespace::set_blob(
    const char *key,
    void *value,
    size_t length) {
  return finish_mutation(
      nvs_set_blob(
          handle(),
          key,
          value,
          length));
}

Flash32Status Flash32Namespace::open(void) {
  return open_namespace(NVS_READWRITE);
}

Flash32Status Flash32Namespace::commit(void) {
  return to_flash32_status(nvs_commit(handle()));
}

Flash32Status Flash32Namespace::erase(const char *key) {
  return finish_mutation(nvs_erase_key(handle(), key));
}

Flash32Status Flash32Namespace::erase_all(void) {
  return finish_mutation(nvs_erase_all(handle()));
}

Flash32Status Flash32Namespace::set_int8(const char *key, int8_t value) {
  return finish_mutation(nvs_set_i8(handle(), key, value));
}

Flash32Status Flash32Namespace::set_uint8(const char *key, uint8_t value) {
  return finish_mutation(nvs_set_u8(handle(), key, value));
}

Flash32Status Flash32Namespace::set_int16(const char *key, int16_t value) {
  return finish_mutation(nvs_set_i16(handle(), key, value));
}

Flash32Status Flash32Namespace::set_uint16(const char *key, uint16_t value) {
  return finish_mutation(nvs_set_u16(handle(), key, value));
}

Flash32Status Flash32Namespace::set_int32(const char *key, int32_t value) {
  return finish_mutation(nvs_set_i32(handle(), key, value));
}

Flash32Status Flash32Namespace::set_uint32(const char *key, uint32_t value) {
  return finish_mutation(nvs_set_u32(handle(), key, value));
}

Flash32Status Flash32Namespace::set_int64(const char *key, int64_t value) {
  return finish_mutation(nvs_set_i64(handle(), key, value));
}

Flash32Status Flash32Namespace::set_uint64(const char *key, uint64_t value) {
  return finish_mutation(nvs_set_u64(handle(), key, value));
}

Flash32Status Flash32Namespace::set_str(const char *key, const char *value) {
  return finish_mutation(nvs_set_str(handle(), key, value));
}

bool Flash32Iterator::advance(void) {
  iterator = nvs_entry_next(iterator);
  return load();
}

bool Flash32Iterator::start(void) {
  iterator = nvs_entry_find(
      NVS_DEFAULT_PART_NAME,
      flash32_namespace.get_name(),
      data_type);
  return iterator && load();
}

bool Flash32Iterator::load(void) {
  if (iterator) {
    nvs_entry_info(iterator, &entry_info);
  }
  return iterator;
}

void Flash32Iterator::stop(void) {
  if (iterator) {
    nvs_release_iterator(iterator);
  }
}

Flash32Iterator::Flash32Iterator(
    const Flash32BaseNamespace &flash32_namespace,
    nvs_type_t data_type) :
      flash32_namespace(flash32_namespace),
      data_type(data_type),
      state(IteratorState::CLOSED),
      iterator(NULL) {
  std::memset(&entry_info, 0, sizeof(entry_info));
}

Flash32Iterator::~Flash32Iterator() {
  stop();
  state = IteratorState::DELETED;
}

bool Flash32Iterator::next(void) {
  bool result = false;

  switch (state) {
  case IteratorState::CLOSED:
    result = start();
    state = result ? IteratorState::RUNNING : IteratorState::FINISHED;
    break;
  case IteratorState::RUNNING:
    result = advance();
    if (!result) {
      state = IteratorState::FINISHED;
    }
    break;

  case IteratorState::FINISHED:
  case IteratorState::DELETED:
    // Nothing to do.
    break;
  }

  return result;
}
