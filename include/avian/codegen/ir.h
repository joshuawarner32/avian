/* Copyright (c) 2008-2014, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef AVIAN_CODEGEN_IR_H
#define AVIAN_CODEGEN_IR_H

namespace avian {
namespace codegen {
namespace ir {

class TargetInfo {
 public:
  unsigned pointerSize;

  explicit TargetInfo(unsigned pointerSize) : pointerSize(pointerSize)
  {
  }
};

class Type {
 public:
  enum class Flavor {
    // A GC-visiible reference
    Object,

    // GC-invisible types
    Integer,
    Float,
    Address,

    // Represents individual halves of two-word types
    // (double/long on 32-bit systems)
    // TODO: remove when possible
    Half,

    // Represents the lack of a return value
    // TODO: remove when possible
    Void,
  };

  typedef int16_t TypeDesc;

 private:
  TypeDesc desc;

 public:
  constexpr Type(Flavor flavor, unsigned size)
      : desc((static_cast<int>(flavor) & 0xff) | ((size & 0xff) << 8))
  {
  }

  inline Flavor flavor() const
  {
    return (Flavor)(desc & 0xff);
  }

  // If the size isn't known without inspecting the TargetInfo, returns -1.
  // Otherwise, matches size(TargetInfo).
  inline int rawSize() const
  {
    return desc >> 8;
  }

  inline unsigned size(const TargetInfo& t) const
  {
    int s = rawSize();
    if (s < 0) {
      return t.pointerSize;
    }
    return (unsigned)s;
  }

  inline bool operator==(const Type& other) const
  {
    return desc == other.desc;
  }

  inline bool operator!=(const Type& other) const
  {
    return !(*this == other);
  }
};

namespace Types {
static constexpr Type Void = Type(Type::Flavor::Void, 0);
static constexpr Type Object = Type(Type::Flavor::Object, -1);
static constexpr Type Iptr = Type(Type::Flavor::Integer, -1);
static constexpr Type I1 = Type(Type::Flavor::Integer, 1);
static constexpr Type I2 = Type(Type::Flavor::Integer, 2);
static constexpr Type I4 = Type(Type::Flavor::Integer, 4);
static constexpr Type I8 = Type(Type::Flavor::Integer, 8);
static constexpr Type F4 = Type(Type::Flavor::Float, 4);
static constexpr Type F8 = Type(Type::Flavor::Float, 8);
static constexpr Type Addr = Type(Type::Flavor::Address, -1);
};

enum class ExtendMode { Signed, Unsigned };

enum class CallingConvention { Native, Avian };

class Value {
 public:
  ir::Type type;

  Value(ir::Type type) : type(type)
  {
  }
};

}  // namespace ir
}  // namespace codegen
}  // namespace avian

#endif  // AVIAN_CODEGEN_IR_H
