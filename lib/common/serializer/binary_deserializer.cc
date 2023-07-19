//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/common/serializer/binary_deserializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/binary_deserializer.hh"

namespace saturn {

void BinaryDeserializer::SetTag(const char *tag) {
  currentTag = tag;
  stack.back().readFieldCount++;
}

//===--------------------------------------------------------------------===//
// complex types
//===--------------------------------------------------------------------===//

void BinaryDeserializer::OnObjectBegin() {
  auto expected_field_count = ReadPOD<u32>();
  auto expected_size = ReadPOD<u64>();
  DCHECK(expected_field_count > 0);
  DCHECK(expected_size > 0);
  stack.emplace_back(expected_field_count, Size(expected_size));
}

void BinaryDeserializer::OnObjectEnd() {
  auto &frame = stack.back();
  if (frame.readFieldCount < frame.expectedFieldCount) {
    throw ParseException("Not all fields were read.");
  }
  stack.pop_back();
}

auto BinaryDeserializer::OnVectorBegin() -> Size { return ReadPOD<Size>(); }

auto BinaryDeserializer::OnMapBegin() -> Size { return ReadPOD<Size>(); }

//===------------------------------------------------------------------------===
// Primitive types
//===------------------------------------------------------------------------===

auto BinaryDeserializer::ReadBool() -> bool { return ReadPOD<bool>(); }

auto BinaryDeserializer::ReadSignedInt8() -> i8 { return ReadPOD<i8>(); }

auto BinaryDeserializer::ReadUnsignedInt8() -> u8 { return ReadPOD<u8>(); }

auto BinaryDeserializer::ReadSignedInt16() -> i16 { return ReadPOD<i16>(); }

auto BinaryDeserializer::ReadUnsignedInt16() -> u16 { return ReadPOD<u16>(); }

auto BinaryDeserializer::ReadSignedInt32() -> i32 { return ReadPOD<i32>(); }

auto BinaryDeserializer::ReadUnsignedInt32() -> u32 { return ReadPOD<u32>(); }

auto BinaryDeserializer::ReadSignedInt64() -> i64 { return ReadPOD<i64>(); }

auto BinaryDeserializer::ReadUnsignedInt64() -> u64 { return ReadPOD<u64>(); }

auto BinaryDeserializer::ReadFloat() -> float { return ReadPOD<float>(); }

auto BinaryDeserializer::ReadDouble() -> double { return ReadPOD<double>(); }

auto BinaryDeserializer::ReadString() -> string {
  auto length = ReadPOD<std::size_t>();
  if (length == 0) {
    return "";
  }
  auto buffer = vector<Datum>(length);
  ReadData(buffer, Size(length));
  return {buffer.begin(), buffer.end()};
}

} // namespace saturn
