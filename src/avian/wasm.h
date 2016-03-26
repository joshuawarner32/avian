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
  }
}

inline bool atomicCompareAndSwap(uintptr_t* p, uintptr_t old, uintptr_t new_)
{
  return atomicCompareAndSwap32(reinterpret_cast<uint32_t*>(p), old, new_);
}

inline void advance(unsigned* stackIndex,
                    unsigned* stackSubIndex,
                    unsigned newStackSubIndex)
{
  abort();
}

inline void push(uint8_t type,
                 uintptr_t* stack,
                 unsigned* stackIndex,
                 unsigned* stackSubIndex,
                 uintptr_t argument)
{
  abort();
}

inline uint64_t dynamicCall(void* function,
                            uintptr_t* arguments,
                            uint8_t* argumentTypes,
                            unsigned argumentCount,
                            unsigned argumentsSize UNUSED,
                            unsigned returnType)
{
  abort();
}

}  // namespace vm

#endif  // WASM_H
