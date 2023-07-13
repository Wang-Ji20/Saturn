//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/string.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

#include <set>

namespace saturn {

class Serializer {
  friend class Writer;
protected:
  virtual void onSerializeBegin() {}
  virtual void onSerializeEnd() {}

  virtual void OnObjectBegin() {}
  virtual void OnObjectEnd() {}

  /// write a generic value
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
  template <typename T> void WriteValue(const T &value) {
    string serializedValue;
    SaturnWriteValue(serializedValue, value);
    OnObjectBegin();
    WriteValue(serializedValue);
    OnObjectEnd();
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
    WriteValue(pair.first);
    WriteValue(pair.second);
  }

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  virtual void OnVectorBegin(Size size) {}
  virtual void OnVectorEnd(Size size) {}
  virtual void OnVectorItemBegin(Size maxSize, Offset curSize) {}
  virtual void OnVectorItemEnd(Size maxSize, Offset curSize) {}

  template <typename T> void WriteValue(const std::vector<T> &vec) {
    const Size vecSize = vec.size();
    OnVectorBegin(vecSize);
    for (Offset i = 0_Offset; i < vecSize; ++i) {
      OnVectorItemBegin(vecSize, i);
      WriteValue(vec[i]);
      OnVectorItemEnd(vecSize, i);
    }
    OnVectorEnd(vecSize);
  }

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  virtual void OnUnorderedMapBegin(Size size) {}
  virtual void OnUnorderedMapEnd(Size size) {}
  virtual void OnUnorderedMapItemBegin() {}
  virtual void OnUnorderedMapItemEnd() {}
  virtual void OnUnorderedMapKeyBegin() {}
  virtual void OnUnorderedMapKeyEnd() {}
  virtual void OnUnorderedMapValueBegin() {}
  virtual void OnUnorderedMapValueEnd() {}

  template <typename K, typename V>
  void WriteValue(const std::unordered_map<K, V> &map) {
    const Size mapSize = map.size();
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
  virtual void OnSetBegin(Size size) {}
  virtual void OnSetEnd(Size size) {}
  virtual void OnSetItemBegin() {}
  virtual void OnSetItemEnd() {}

  template <typename T> void WriteValue(const std::set<T> &set) {
    const Size setSize = set.size();
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
	virtual void WriteValue(const char *value);
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

  std::vector<u8> buffer;
};

} // namespace saturn
