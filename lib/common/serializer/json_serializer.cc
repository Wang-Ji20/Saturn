//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/common/serializer/json_serializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/json_serializer.hh"

namespace saturn {

void JsonSerializer::PushValue(yyjson_mut_val *val) {
  auto *current = GetCurrent();
  if (yyjson_mut_is_arr(current)) {
    yyjson_mut_arr_append(current, val);
    return;
  }
  if (yyjson_mut_is_obj(current)) {
    yyjson_mut_obj_add(current, current_tag, val);
    return;
  }
  SaturnUnreachable();
}

/// tag: the key of the value
void JsonSerializer::SetTag(const char *tag) {
  current_tag = yyjson_mut_strcpy(doc, tag);
}

//===------------------------------------------------===
// Nested types
//===------------------------------------------------===

void JsonSerializer::OnObjectBegin() {
  auto *newValue = yyjson_mut_obj(doc);
  PushValue(newValue);
  stack.push_back(newValue);
}

void JsonSerializer::PruneEmptyObject(yyjson_mut_val *obj) {
  auto *parent = GetCurrent();

  // parent is array, remove obj
  if (yyjson_mut_is_arr(parent)) {
    size_t idx;
    size_t max;
    yyjson_mut_val *item;
    size_t found;
    yyjson_mut_arr_foreach(parent, idx, max, item) {
      if (item == obj) {
        found = idx;
      }
    }
    yyjson_mut_arr_remove(parent, found);
    return;
  }

  // parent is object, remove obj
  if (!yyjson_mut_is_obj(parent)) {
    return;
  }
  size_t idx;
  size_t max;
  yyjson_mut_val *item;
  yyjson_mut_val *key;
  const char *found;
  yyjson_mut_obj_foreach(parent, idx, max, key, item) {
    if (item == obj) {
      found = yyjson_mut_get_str(key);
    }
  }
  yyjson_mut_obj_remove_key(parent, found);
}

void JsonSerializer::OnObjectEnd() {
  auto *obj = GetCurrent();
  auto count = yyjson_mut_obj_size(obj);
  stack.pop_back();
  if (count == 0 && skipEmpty_ && !stack.empty()) {
    PruneEmptyObject(obj);
  }
}

void JsonSerializer::OnPairBegin() {
  auto *newValue = yyjson_mut_obj(doc);
  PushValue(newValue);
  stack.push_back(newValue);
}

void JsonSerializer::OnPairEnd() { stack.pop_back(); }

void JsonSerializer::OnPairKeyBegin() { SetTag("key"); }

void JsonSerializer::OnPairValueBegin() { SetTag("value"); }

void JsonSerializer::OnVectorBegin(Size size) {
  auto *newValue = yyjson_mut_arr(doc);
  if (size != 0_Size || !skipEmpty_) {
    PushValue(newValue);
  }
  stack.push_back(newValue);
}

void JsonSerializer::OnVectorEnd(Size size) { stack.pop_back(); }

void JsonSerializer::OnUnorderedMapBegin(Size size) {
  auto *newValue = yyjson_mut_arr(doc);
  if (size != 0_Size || !skipEmpty_) {
    PushValue(newValue);
  }
  stack.push_back(newValue);
}

void JsonSerializer::OnUnorderedMapEnd(Size size) { stack.pop_back(); }

void JsonSerializer::OnUnorderedMapItemBegin() {
  auto *newValue = yyjson_mut_obj(doc);
  PushValue(newValue);
  stack.push_back(newValue);
}

void JsonSerializer::OnUnorderedMapItemEnd() { stack.pop_back(); }

void JsonSerializer::OnUnorderedMapKeyBegin() { SetTag("key"); }

void JsonSerializer::OnUnorderedMapValueBegin() { SetTag("value"); }

//===------------------------------------------------===
// primitive types
//===------------------------------------------------===

void JsonSerializer::WriteNull() {
  auto *val = yyjson_mut_null(doc);
  PushValue(val);
}

void JsonSerializer::WriteValue(bool value) {
  auto *val = yyjson_mut_bool(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(int8_t value) {
  auto *val = yyjson_mut_sint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(uint8_t value) {
  auto *val = yyjson_mut_uint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(int16_t value) {
  auto *val = yyjson_mut_sint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(uint16_t value) {
  auto *val = yyjson_mut_uint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(int32_t value) {
  auto *val = yyjson_mut_sint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(uint32_t value) {
  auto *val = yyjson_mut_uint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(int64_t value) {
  auto *val = yyjson_mut_sint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(uint64_t value) {
  auto *val = yyjson_mut_uint(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(float value) {
  auto *val = yyjson_mut_real(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(double value) {
  auto *val = yyjson_mut_real(doc, value);
  PushValue(val);
}

void JsonSerializer::WriteValue(const char *value) {
  if (skipEmpty_ && strlen(value) == 0) {
    return;
  }
  auto *val = yyjson_mut_strcpy(doc, value);
  PushValue(val);
}

} // namespace saturn
