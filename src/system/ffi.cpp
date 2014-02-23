/* Copyright (c) 2008-2013, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include <avian/common.h>
#include <avian/arch.h>

namespace avian {
namespace system {
namespace ffi {

uint64_t call(void* function,
              uintptr_t* arguments,
              uint8_t* types,
              unsigned count,
              unsigned size,
              unsigned returnType)
{
  return vm::dynamicCall(function, arguments, types, count, size, returnType);
}

}  // namespace ffi
}  // namespace system
}  // namespace avian
