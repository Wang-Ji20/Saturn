//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/serializer/json_serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/logger.hh"
#include "common/serializer/serializer.hh"

#include "yyjson.h"

#include <vector>

namespace saturn {

class JsonSerializer : public Serializer {
  //===------------------------------------------------===
  // public interface
  //===------------------------------------------------===
public:
  auto GetRoot() -> yyjson_mut_val * {
    DCHECK(stack.size() == 1);
    return stack.front();
  }

  template <typename T>
  static auto Serialize(const T &value,
                        yyjson_mut_doc *doc,
                        bool pretty = true,
                        bool skipNull = false,
                        bool skipEmpty = false) -> yyjson_mut_val * {
    JsonSerializer serializer(doc, pretty, skipNull, skipEmpty);
    serializer.WriteValue(value);
    return serializer.GetRoot();
  }

  template <typename T>
  static auto ToString(const T &value,
                       bool pretty = true,
                       bool skipNull = false,
                       bool skipEmpty = false) -> std::string {
    auto *doc = yyjson_mut_doc_new(nullptr);
    auto *root = Serialize(value, doc, pretty, skipNull, skipEmpty);
    auto *str =
        yyjson_mut_write(doc, pretty ? YYJSON_WRITE_PRETTY : 0, nullptr);
    DCHECK(str != nullptr) << "malformed json\n";
    auto result = std::string(str);
    yyjson_mut_doc_free(doc);
    return result;
  }

  //===------------------------------------------------===
  // Internals
  //===------------------------------------------------===

private:
  explicit JsonSerializer(yyjson_mut_doc *doc,
                          bool pretty = true,
                          bool skipNull = false,
                          bool skipEmpty = false)
      : doc(doc),
        stack{yyjson_mut_arr(doc)},
        pretty_(pretty),
        skipNull_(skipNull),
        skipEmpty_(skipEmpty) {
    yyjson_mut_doc_set_root(doc, GetCurrent());
  }

  yyjson_mut_doc *doc;
  yyjson_mut_val *current_tag;
  std::vector<yyjson_mut_val *> stack;

  bool pretty_ = true;
  bool skipNull_ = false;
  bool skipEmpty_ = false;

  inline auto GetCurrent() -> yyjson_mut_val * { return stack.back(); }

  void PushValue(yyjson_mut_val *val);
  void PruneEmptyObject(yyjson_mut_val *obj);

  //===------------------------------------------------===
  // Serializer interface
  //===------------------------------------------------===

public:
  void SetTag(const char *tag);

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
