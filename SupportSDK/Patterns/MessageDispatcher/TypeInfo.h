#pragma once

#include <typeinfo>
#include <xrefwrap>

typedef std::reference_wrapper<const std::type_info> TypeInfoRef;

struct Hasher 
  {
  std::size_t operator () (TypeInfoRef code) const
    {
    return code.get().hash_code();
    }
  };

struct EqualTo 
  {
  bool operator () (TypeInfoRef lhs, TypeInfoRef rhs) const
    {
    return lhs.get() == rhs.get();
    }
  };