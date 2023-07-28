//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// The serializer interface is only used to show the
// layout of data. It is not responsible for the
// specific formatting of the data.
//
// Such thing is left to the concrete serializer, and
// caller should aware what type of serializer is used in
// storage module.
//===------------------------------------------------===

#pragma once

#include "common/string.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

#include <set>

namespace saturn {

class Serializer {
  friend struct Writer;

public:
  virtual void SetTag(const char *tag) = 0;

  /// write properties
  /// syntax sugar for "key" : "value"
  template <typename T> void WriteProperty(const char *key, const T &value) {
    SetTag(key);
    if constexpr (std::enable_if<!std::is_enum<T>::value>::value) {
      WriteValue(value);
      return;
    }
    WriteValue(static_cast<typename std::underlying_type<T>::type>(value));
  }

  virtual void OnObjectBegin() {}
  virtual void OnObjectEnd() {}

  /// write a generic value
  // Friend Template Argument-Dependent Lookup Extension
  //
  // Usage:
  // --------------------
  //
  // in a user-defined type:
  //
  // class xxx {
  //
  // template <typename Ser>
  // friend void SaturnWriteValue(Ser& serializer, const xxx& value) {
  //    put the serialization result into serializer
  // }
  //
  // };
  //
  // Note:
  // --------------------
  // 1. the user-defined type can be a scalar or a complex type, so we left
  //    OnObjectBegin() and OnObjectEnd() to the user.
  //
  template <typename T> void WriteValue(const T &value) {
    SaturnWriteValue(*this, value);
  }

  /// write a unique_ptr
  template <typename T> void WriteValue(const unique_ptr<T> &ptr) {
    WriteValue(ptr.get());
  }

  /// write a pointer
  template <typename T>
  auto WriteValue(const T ptr) ->
      typename std::enable_if<std::is_pointer<T>::value, void>::type {
    if (ptr == nullptr) {
      WriteNull();
    } else {
      WriteValue(*ptr);
    }
  }

  //===------------------------------------------------===
  // write a pair
  //===------------------------------------------------===
  virtual void OnPairBegin() {}
  virtual void OnPairKeyBegin() {}
  virtual void OnPairKeyEnd() {}
  virtual void OnPairValueBegin() {}
  virtual void OnPairValueEnd() {}
  virtual void OnPairEnd() {}

  template <typename K, typename V>
  void WriteValue(const std::pair<K, V> &pair) {
    OnPairBegin();
    OnPairKeyBegin();
    WriteValue(pair.first);
    OnPairKeyEnd();
    OnPairValueBegin();
    WriteValue(pair.second);
    OnPairValueEnd();
    OnPairEnd();
  }

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  virtual void OnVectorBegin(MemoryByte size) {}
  virtual void OnVectorEnd(MemoryByte size) {}
  virtual void OnVectorItemBegin(MemoryByte maxSize, MemoryByte curSize) {}
  virtual void OnVectorItemEnd(MemoryByte maxSize, MemoryByte curSize) {}

  template <typename T> void WriteValue(const std::vector<T> &vec) {
    const MemoryByte vecSize = vec.size();
    OnVectorBegin(vecSize);
    for (auto i = 0ULL; i < vecSize; ++i) {
      OnVectorItemBegin(vecSize, i);
      WriteValue(vec[i]);
      OnVectorItemEnd(vecSize, i);
    }
    OnVectorEnd(vecSize);
  }

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  virtual void OnUnorderedMapBegin(MemoryByte size) {}
  virtual void OnUnorderedMapEnd(MemoryByte size) {}
  virtual void OnUnorderedMapItemBegin() {}
  virtual void OnUnorderedMapItemEnd() {}
  virtual void OnUnorderedMapKeyBegin() {}
  virtual void OnUnorderedMapKeyEnd() {}
  virtual void OnUnorderedMapValueBegin() {}
  virtual void OnUnorderedMapValueEnd() {}

  template <typename K, typename V>
  void WriteValue(const std::unordered_map<K, V> &map) {
    const MemoryByte mapSize = map.size();
    OnUnorderedMapBegin(mapSize);
    for (auto &item : map) {
      OnUnorderedMapItemBegin();
      OnUnorderedMapKeyBegin();
      WriteValue(item.first);
      OnUnorderedMapKeyEnd();
      OnUnorderedMapValueBegin();
      WriteValue(item.second);
      OnUnorderedMapValueEnd();
      OnUnorderedMapItemEnd();
    }
    OnUnorderedMapEnd(mapSize);
  }

  //===------------------------------------------------===
  // write a set
  //===------------------------------------------------===
  virtual void OnSetBegin(MemoryByte size) {}
  virtual void OnSetEnd(MemoryByte size) {}
  virtual void OnSetItemBegin() {}
  virtual void OnSetItemEnd() {}

  template <typename T> void WriteValue(const std::set<T> &set) {
    const MemoryByte setSize = set.size();
    OnSetBegin(setSize);
    for (auto &item : set) {
      OnSetItemBegin();
      WriteValue(item);
      OnSetItemEnd();
    }
    OnSetEnd(setSize);
  }

  //===------------------------------------------------===
  // primitive types
  //===------------------------------------------------===

  virtual void WriteValue(const string &value);
  virtual void WriteValue(const char *value) = 0;
  virtual void WriteNull() = 0;
  virtual void WriteValue(bool value) = 0;
  virtual void WriteValue(u8 value) = 0;
  virtual void WriteValue(i8 value) = 0;
  virtual void WriteValue(u16 value) = 0;
  virtual void WriteValue(i16 value) = 0;
  virtual void WriteValue(u32 value) = 0;
  virtual void WriteValue(i32 value) = 0;
  virtual void WriteValue(u64 value) = 0;
  virtual void WriteValue(i64 value) = 0;
  virtual void WriteValue(float value) = 0;
  virtual void WriteValue(double value) = 0;
};

} // namespace saturn
