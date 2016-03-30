/* Copyright (c) 2008-2015, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef WASM_H
#define WASM_H

#include "avian/types.h"
#include "avian/common.h"
#include <avian/util/runtime-array.h>

#define VA_LIST(x) (&(x))

extern "C" uint64_t vmNativeCall(void* function,
                                 unsigned stackTotal,
                                 void* memoryTable,
                                 unsigned memoryCount,
                                 void* gprTable,
                                 void* vfpTable,
                                 unsigned returnType);

namespace vm {

inline void trap()
{
  abort();
}

// todo: determine the minimal operation types and domains needed to
// implement the following barriers (see
// http://community.arm.com/groups/processors/blog/2011/10/19/memory-access-ordering-part-3--memory-access-ordering-in-the-arm-architecture).
// For now, we just use DMB SY as a conservative but not necessarily
// performant choice.

inline void memoryBarrier()
{
}

inline void storeStoreMemoryBarrier()
{
}

inline void storeLoadMemoryBarrier()
{
}

inline void loadMemoryBarrier()
{
}

inline bool atomicCompareAndSwap32(uint32_t* p, uint32_t old, uint32_t new_)
{
  if(*p == old) {
    *p = new_;
    return true;
  }
  return false;
}

inline bool atomicCompareAndSwap(uintptr_t* p, uintptr_t old, uintptr_t new_)
{
  return atomicCompareAndSwap32(reinterpret_cast<uint32_t*>(p), old, new_);
}

inline void advance(unsigned* stackIndex UNUSED,
                    unsigned* stackSubIndex UNUSED,
                    unsigned newStackSubIndex UNUSED)
{
  abort();
}

inline void push(uint8_t type UNUSED,
                 uintptr_t* stack UNUSED,
                 unsigned* stackIndex UNUSED,
                 unsigned* stackSubIndex UNUSED,
                 uintptr_t argument UNUSED)
{
  abort();
}

inline uint64_t dynamicCall(void* function UNUSED,
                            uintptr_t* arguments UNUSED,
                            uint8_t* argumentTypes UNUSED,
                            unsigned argumentCount UNUSED,
                            unsigned argumentsSize UNUSED,
                            unsigned returnType UNUSED)
{
  abort();
}

}  // namespace vm

#endif  // WASM_H
