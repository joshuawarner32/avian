/* Copyright (c) 2008-2014, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef APPEND_H
#define APPEND_H

#include <avian/common.h>
#include <avian/util/allocator.h>

namespace vm {

inline const char* append(avian::util::Allocator* allocator,
                          avian::util::String a,
                          avian::util::String b,
                          avian::util::String c)
{
  char* p = static_cast<char*>(allocator->allocate((a.count + b.count + c.count) + 1));
  memcpy(p, a.begin(), a.count);
  memcpy(p + a.count, b.begin(), b.count);
  memcpy(p + a.count + b.count, c.begin(), c.count + 1);
  return p;
}

inline const char* append(avian::util::Allocator* allocator,
                          avian::util::String a,
                          avian::util::String b)
{
  char* p = static_cast<char*>(allocator->allocate((a.count + b.count) + 1));
  memcpy(p, a.begin(), a.count);
  memcpy(p + a.count, b.begin(), b.count + 1);
  return p;
}

inline const char* copy(avian::util::Allocator* allocator, avian::util::String a)
{
  char* p = static_cast<char*>(allocator->allocate(a.count + 1));
  memcpy(p, a.begin(), a.count + 1);
  return p;
}

}  // namespace vm

#endif  // APPEND_H
