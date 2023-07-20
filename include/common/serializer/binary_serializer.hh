//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/binary_serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/serializer/serializer.hh"
#include "container/vector.hh"

namespace saturn {

/// Serialize format:
/// 1. For primitive types, we use the binary format of the type.
/// 2. For object types, we use the following format:
///    1. The first 4 bytes is the field count.
///    2. The next 8 bytes is the size of the object.
/// 3. for STL container types, we use the following format:
///    1. The first 4 bytes is the container size
///    2. The next are elements in the container
class BinarySerializer : public Serializer {
private:
  //===------------------------------------------------------------------------===
  // Internal Implementation
  //===------------------------------------------------------------------------===

  struct State {
    u32 fieldCount;
    Size objectSize;
    Offset bufferOffset;
    State(u32 fieldCount, Size objectSize, Offset bufferOffset)
        : fieldCount(fieldCount),
          objectSize(objectSize),
          bufferOffset(bufferOffset) {}
  };

  const char *current_tag;

  vector<Datum> data;

  vector<State> stack;

  template <typename T> void Store(const T &value, Datum *buffer) {
    std::memcpy(buffer, (void *)&value, sizeof(T));
  }

  template <typename T> void Write(T value) {
    static_assert(std::is_trivially_destructible<T>::value,
                  "T must be trivally destructible-typed variable");
    WriteData(reinterpret_cast<const char *>(&value), Size(sizeof(T)));
  }

  void WriteData(const Datum *buffer, Size size) {
    data.insert(data.end(), buffer, buffer + size);
    stack.back().objectSize += size;
  }

  void WriteData(const char *buffer, Size size) {
    WriteData(reinterpret_cast<const Datum *>(buffer), size);
  }

  explicit BinarySerializer() { stack.emplace_back(0, 0_Size, 0_Offset); };

  //===------------------------------------------------------------------------===
  // parent interface
  //===------------------------------------------------------------------------===
public:
  template <typename T> static auto Serialize(T &obj) -> vector<Datum> {
    BinarySerializer serializer;
    serializer.WriteValue(obj);
    return std::move(serializer.data);
  }

  void SetTag(const char *tag) override;

  void OnObjectBegin() final;
  void OnObjectEnd() final;

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  void OnVectorBegin(Size size) final;

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  void OnUnorderedMapBegin(Size size) final;

  // https://stackoverflow.com/questions/3678197/virtual-function-implemented-in-base-class-not-being-found-by-compiler
  using Serializer::WriteValue;
  void WriteValue(const char *value) final;
  void WriteNull() final;
  void WriteValue(bool value) final;
  void WriteValue(u8 value) final;
  void WriteValue(i8 value) final;
  void WriteValue(u16 value) final;
  void WriteValue(i16 value) final;
  void WriteValue(u32 value) final;
  void WriteValue(i32 value) final;
  void WriteValue(u64 value) final;
  void WriteValue(i64 value) final;
  void WriteValue(float value) final;
  void WriteValue(double value) final;
};

} // namespace saturn
