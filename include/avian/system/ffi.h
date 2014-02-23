  /* Copyright (c) 2008-2013, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef AVIAN_SYSTEM_FFI_H
#define AVIAN_SYSTEM_FFI_H

#include <stdint.h>

#include <avian/common.h>

namespace avian {
namespace system {
namespace ffi {

// Call a native function dynamically
// TODO: document allowed types / switch to enums
uint64_t call(void* function,
              uintptr_t* arguments,
              uint8_t* types,
              unsigned count,
              unsigned size,
              unsigned returnType);

} // namespace ffi
} // namespace system
} // namespace avian

#endif
