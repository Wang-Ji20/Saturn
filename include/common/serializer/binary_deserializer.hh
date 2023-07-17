//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/binary_deserializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/serializer/deserializer.hh"

namespace saturn {

class BinaryDeserializer : public Deserializer {

protected:
  //===------------------------------------------------------------------------===
  // override functions
  //===------------------------------------------------------------------------===
  void SetTag(const char *tag) override;
  auto OnVectorBegin() -> Size override;
  void OnVectorEnd() override;
  auto OnMapBegin() -> Size override;
  void OnMapEnd() override;
  void OnMapEntryBegin() override;
  void OnMapEntryEnd() override;
  void OnMapKeyBegin() override;
  void OnMapKeyEnd() override;
  void OnMapValueBegin() override;
  void OnMapValueEnd() override;
  void OnObjectBegin() override;
  void OnObjectEnd() override;
  void OnPairBegin() override;
  void OnPairKeyBegin() override;
  void OnPairKeyEnd() override;
  void OnPairValueBegin() override;
  void OnPairValueEnd() override;
  void OnPairEnd() override;

  auto ReadBool() -> bool override;
  auto ReadSignedInt8() -> i8 override;
  auto ReadUnsignedInt8() -> u8 override;
  auto ReadSignedInt16() -> i16 override;
  auto ReadUnsignedInt16() -> u16 override;
  auto ReadSignedInt32() -> i32 override;
  auto ReadUnsignedInt32() -> u32 override;
  auto ReadSignedInt64() -> i64 override;
  auto ReadUnsignedInt64() -> u64 override;
  auto ReadFloat() -> float override;
  auto ReadDouble() -> double override;
  auto ReadString() -> string override;
};

} // namespace saturn
