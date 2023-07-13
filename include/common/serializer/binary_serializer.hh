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

namespace saturn {

class BinarySerializer : public Serializer {
protected:
  void onSerializeBegin() override;
  void onSerializeEnd() override;

  void OnObjectBegin() override;
  void OnObjectEnd() override;

  //===------------------------------------------------===
  // write a pair
  //===------------------------------------------------===
  void OnPairBegin() override;
  void OnPairKeyBegin() override;
  void OnPairKeyEnd() override;
  void OnPairValueBegin() override;
  void OnPairValueEnd() override;
  void OnPairEnd() override;

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  void OnVectorBegin(Size size) override;
  void OnVectorEnd(Size size) override;
  void OnVectorItemBegin(Size maxSize, Offset curSize) override;
  void OnVectorItemEnd(Size maxSize, Offset curSize) override;

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  void OnUnorderedMapBegin(Size size) override;
  void OnUnorderedMapEnd(Size size) override;
  void OnUnorderedMapItemBegin() override;
  void OnUnorderedMapItemEnd() override;
  void OnUnorderedMapKeyBegin() override;
  void OnUnorderedMapKeyEnd() override;
  void OnUnorderedMapValueBegin() override;
  void OnUnorderedMapValueEnd() override;

  //===------------------------------------------------===
  // write a set
  //===------------------------------------------------===
  void OnSetBegin(Size size) override;
  void OnSetEnd(Size size) override;
  void OnSetItemBegin() override;
  void OnSetItemEnd() override;

  void WriteNull() override;
  void WriteValue(bool value) override;
  void WriteValue(u8 value) override;
  void WriteValue(i8 value) override;
  void WriteValue(u16 value) override;
  void WriteValue(i16 value) override;
  void WriteValue(u32 value) override;
  void WriteValue(i32 value) override;
  void WriteValue(u64 value) override;
  void WriteValue(i64 value) override;
  void WriteValue(float value) override;
  void WriteValue(double value) override;
};

} // namespace saturn
