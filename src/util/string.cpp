/* Copyright (c) 2008-2014, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include <string.h>

#include <avian/util/string.h>

namespace avian {
namespace util {

String::String(const char* text) : Slice<const char>(text, strlen(text))
{
}

bool String::operator==(const String& o) const
{
  return count == o.count && memcmp(items, o.items, count) == 0;
}

}  // namespace util
}  // namespace avian
