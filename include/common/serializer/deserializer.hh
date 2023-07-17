//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/deserializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------------------------------===
// The deserializer interface.
//===------------------------------------------------------------------------===

#pragma once

#include "common/serializer/serializer_type_traits.hh"
#include "common/string.hh"

namespace saturn {

template <typename T, template <typename S = T> class Predicate = is_unique_ptr>
using ReturnType = typename std::enable_if<Predicate<T>::value, T>::type;

class Deserializer {

  //===------------------------------------------------------------------------===
  // class interface
  //===------------------------------------------------------------------------===
protected:
  virtual void SetTag(const char *tag) = 0;
  virtual void SetTag(const string &tag) { SetTag(tag.c_str()); };

  template <typename T> auto Read() -> ReturnType<T, is_deserializable> {
    return T::SaturnReadValue(*this);
  }

  template <typename T> auto Read() -> ReturnType<T, is_unique_ptr> {
    using ELEMENT_TYPE = typename is_unique_ptr<T>::ELEMENT_TYPE;
    return make_unique<ELEMENT_TYPE>(Read<ELEMENT_TYPE>());
  }

  template <typename T> auto Read() -> ReturnType<T, is_shared_ptr> {
    using ELEMENT_TYPE = typename is_shared_ptr<T>::ELEMENT_TYPE;
    return make_shared<ELEMENT_TYPE>(Read<ELEMENT_TYPE>());
  }
};

} // namespace saturn
