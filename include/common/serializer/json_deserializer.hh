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
  static auto FromString(const std::string &str) -> JsonDeserializer {
    auto *doc = yyjson_read(str.c_str(), str.size(), 0);
    DCHECK(doc != nullptr) << "malformed json\n";
    return {yyjson_doc_get_root(doc), doc};
  }

  ~JsonDeserializer() { yyjson_doc_free(doc); }
private:
  JsonDeserializer(yyjson_val *val, yyjson_doc *doc) : doc(doc) {
    stack.emplace_back(val);
  }

  yyjson_doc *doc = nullptr;
  const char *currentTag = nullptr;

  struct StackFrame {
    yyjson_val *val;
    yyjson_arr_iter arr_iter;
    explicit StackFrame(yyjson_val *val) : val(val) {
      yyjson_arr_iter_init(val, &arr_iter);
    }
  };

  vector<StackFrame> stack;
  void DumpDoc();
  void DumpCurrent();
  static void Dump(yyjson_mut_val *val);
  static void Dump(yyjson_val *val);
  void ThrowTypeError(yyjson_val *val, const char *expected);

	auto GetNextValue() -> yyjson_val *;
  inline auto GetCurrent() -> StackFrame & { return stack.back(); }
  inline void Push(yyjson_val *val) { stack.emplace_back(val); }
  inline void Pop() { stack.pop_back(); }


protected:
  //===------------------------------------------------------------------------===
  // override functions
  //===------------------------------------------------------------------------===
  inline void SetTag(const char *tag) override { currentTag = tag; }
  auto OnVectorBegin() -> MemoryByte override;
  void OnVectorEnd() override;
  auto OnMapBegin() -> MemoryByte override;
  void OnMapEnd() override;
  void OnMapEntryBegin() override;
  void OnMapEntryEnd() override;
  void OnMapKeyBegin() override;
  void OnMapValueBegin() override;
  void OnObjectBegin() override;
  void OnObjectEnd() override;
  void OnPairBegin() override;
  void OnPairKeyBegin() override;
  void OnPairValueBegin() override;
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
