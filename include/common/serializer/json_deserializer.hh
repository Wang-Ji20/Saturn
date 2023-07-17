//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/json_deserializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/serializer/deserializer.hh"

#include "yyjson.h"

namespace saturn {

class JsonDeserializer : public Deserializer {
public:
  JsonDeserializer(yyjson_val *val, yyjson_doc *doc) : doc(doc) {
    stack.emplace_back(val);
  }
  ~JsonDeserializer() { yyjson_doc_free(doc); }

private:
  yyjson_doc *doc = nullptr;
  const char *current_tag = nullptr;

  struct StackFrame {
    yyjson_val *val;
    yyjson_arr_iter arr_iter;
    explicit StackFrame(yyjson_val *val) : val(val) {
      yyjson_arr_iter_init(val, &arr_iter);
    }
  };

  vector<StackFrame> stack;

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
