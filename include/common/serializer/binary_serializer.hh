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



//===------------------------------------------------------------------------===
// parent interface
//===------------------------------------------------------------------------===
protected:
  void SetTag(const char *tag) override;

  void OnObjectBegin() final;
  void OnObjectEnd() final;

  //===------------------------------------------------===
  // write a pair
  //===------------------------------------------------===
  void OnPairBegin() final;
  void OnPairKeyBegin() final;
  void OnPairValueBegin() final;
  void OnPairEnd() final;

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  void OnVectorBegin(Size size) final;
  void OnVectorEnd(Size size) final;

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  void OnUnorderedMapBegin(Size size) final;
  void OnUnorderedMapEnd(Size size) final;
  void OnUnorderedMapItemBegin() final;
  void OnUnorderedMapItemEnd() final;
  void OnUnorderedMapKeyBegin() final;
  void OnUnorderedMapValueBegin() final;

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
