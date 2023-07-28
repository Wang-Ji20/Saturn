//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/common/serializer/binary_serializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/binary_serializer.hh"

namespace saturn {

void BinarySerializer::SetTag(const char *tag) {
  current_tag = tag;
  stack.back().fieldCount++;
}

//===------------------------------------------------------------------------===
// complex types
//===------------------------------------------------------------------------===

void BinarySerializer::OnVectorBegin(MemoryByte size) {
  Write<MemoryByte>(size);
}

void BinarySerializer::OnUnorderedMapBegin(MemoryByte size) {
  Write<MemoryByte>(size);
}

void BinarySerializer::OnObjectBegin() {
  stack.emplace_back(0, 0ULL, data.size());
  Write<u32>(0);           // Placeholder for the field count
  Write<MemoryByte>(0ULL); // Placeholder for the size
}

void BinarySerializer::OnObjectEnd() {
  auto &frame = stack.back();
  // Patch the field count and size
  auto *message_start = &data[frame.bufferOffset];
  Store<u32>(frame.fieldCount, message_start);
  Store<MemoryByte>(frame.objectSize, message_start + sizeof(u32));
  stack.pop_back();
}

//===------------------------------------------------------------------------===
// primitive types
//===------------------------------------------------------------------------===

void BinarySerializer::WriteNull() {}

void BinarySerializer::WriteValue(bool value) { Write<bool>(value); }

void BinarySerializer::WriteValue(u8 value) { Write<u8>(value); }

void BinarySerializer::WriteValue(u16 value) { Write<u16>(value); }

void BinarySerializer::WriteValue(u32 value) { Write<u32>(value); }

void BinarySerializer::WriteValue(u64 value) { Write<u64>(value); }

void BinarySerializer::WriteValue(i8 value) { Write<i8>(value); }

void BinarySerializer::WriteValue(i16 value) { Write<i16>(value); }

void BinarySerializer::WriteValue(i32 value) { Write<i32>(value); }

void BinarySerializer::WriteValue(i64 value) { Write<i64>(value); }

void BinarySerializer::WriteValue(float value) { Write<float>(value); }

void BinarySerializer::WriteValue(double value) { Write<double>(value); }

void BinarySerializer::WriteValue(const char *value) {
  auto length = strlen(value);
  Write<size_t>(length);
  WriteData(value, length);
}

} // namespace saturn
