/* Copyright (c) 2008-2015, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include <avian/tools/object-writer/tools.h>

namespace {

using namespace avian::tools;
using namespace avian::util;

template <unsigned BytesPerWord, PlatformInfo::Architecture Architecture>
class WasmPlatform : public Platform {
 public:
  virtual bool writeObject(OutputStream* out,
                           Slice<SymbolInfo> symbols,
                           Slice<const uint8_t> data,
                           unsigned accessFlags,
                           unsigned alignment)
  {
    return true;
  }

  WasmPlatform() : Platform(PlatformInfo(PlatformInfo::Wasm, Architecture))
  {
  }
};

WasmPlatform<4, PlatformInfo::Wasm32> wasm32Platform;

}  // namespace
