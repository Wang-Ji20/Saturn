//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/unique_ptr.hh"
#include "common/string.hh"

namespace saturn {

class Serializer {
protected:
  template <typename T> void WriteValue(const T &value) {
    WriteValue(value);
  }

  template <typename T> void WriteValue(const unique_ptr<T> &ptr) {
    WriteValue(ptr.get());
  }

  template <typename T>
  auto WriteValue(const T ptr) ->
      typename std::enable_if<std::is_pointer<T>::value, void>::type {
    if (ptr == nullptr) {
      WriteNull();
    } else {
      WriteValue(*ptr);
    }
  }

  // Friend Template ADL Extension
  // Usage:
  //
  // in a user-defined type:
  //
  // class xxx {
  //
  // template <typename T>
  // friend void SaturnWriteValue(string& serializer, const T& value) {
  //    put the serialization result into serializer
  // }
  //
  // };
  //
  template <typename T>
  void WriteValue(string& serializedValue, const T& value) {
    SaturnWriteValue(serializedValue, value);
    WriteValue(serializedValue);
  }

  virtual void WriteNull() = 0;
};



} // namespace saturn
