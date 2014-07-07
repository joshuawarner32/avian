/* Copyright (c) 2008-2014, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef AVIAN_UTIL_STRING_H
#define AVIAN_UTIL_STRING_H

#include "slice.h"

namespace avian {
namespace util {

class String : public Slice<const char> {
 public:
  String(const char* text);
  inline String(const char* text, size_t length)
      : Slice<const char>(text, length)
  {
  }

  bool operator==(const String& o) const;
};

inline bool operator==(const String& a, const char* b)
{
  return a == String(b);
}
inline bool operator==(const char* a, const String& b)
{
  return String(a) == b;
}

}  // namespace util
}  // namespace avian

#endif  // AVIAN_UTIL_STRING_H
