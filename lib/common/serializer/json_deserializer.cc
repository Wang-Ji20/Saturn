//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/common/serializer/json_deserializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/json_deserializer.hh"

namespace saturn {

auto JsonDeserializer::GetNextValue() -> yyjson_val * {
  auto &current = GetCurrent();
  // current is either an array or an object
  yyjson_val *val = nullptr;
  if (yyjson_is_obj(current.val)) {
    val = yyjson_obj_get(current.val, currentTag);
    if (val == nullptr) {
      const char *json = yyjson_val_write(GetCurrent().val, 0, nullptr);
      string currentObj{json};
      free((void *)json);
      throw ParseException(
          format("Cannot find tag: %s in object: %s", currentTag, currentObj));
    }
    return val;
  }

  if (!yyjson_is_arr(current.val)) {
    throw ParseException(format("Expect array or object, but got: %v",
                                yyjson_get_type(current.val)));
  }

  // current is an array
  val = yyjson_arr_iter_next(&current.arr_iter);
  if (val == nullptr) {
    const char *json = yyjson_val_write(GetCurrent().val, 0, nullptr);
    string currentArr{json};
    free((void *)json);
    throw ParseException(
        format("Expect more elements in array: %v", currentArr));
  }
  return val;
}

void JsonDeserializer::DumpDoc() {
  const char *json = yyjson_write(doc, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void JsonDeserializer::DumpCurrent() {
  const char *json = yyjson_val_write(GetCurrent().val, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void JsonDeserializer::Dump(yyjson_mut_val *val) {
  const char *json = yyjson_mut_val_write(val, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void JsonDeserializer::Dump(yyjson_val *val) {
  const char *json = yyjson_val_write(val, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void JsonDeserializer::ThrowTypeError(yyjson_val *val, const char *expected) {
  const auto *actual = yyjson_get_type_desc(val);
  auto &parent = GetCurrent();
  if (yyjson_is_obj(parent.val)) {
    throw ParseException(
        format("Property: %s expected type: %s, but got type: %s",
               currentTag,
               expected,
               actual));
  }
  if (yyjson_is_arr(parent.val)) {
    throw ParseException(
        format("Sequence expect child of type: %s, but got type: %s",
               expected,
               actual));
  }
  throw InternalException(
      "Cannot get nested value from non object or array-type");
}

//===------------------------------------------------------------------------===
// override hooks
//===------------------------------------------------------------------------===

auto JsonDeserializer::OnVectorBegin() -> Size {
  auto *val = GetNextValue();
  if (!yyjson_is_arr(val)) {
    ThrowTypeError(val, "array");
  }
  Push(val);
  return Size(yyjson_arr_size(val));
}

void JsonDeserializer::OnVectorEnd() { Pop(); }

auto JsonDeserializer::OnMapBegin() -> Size {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "array");
  }
  Push(val);
  return Size(yyjson_obj_size(val));
}

void JsonDeserializer::OnMapEnd() { Pop(); }

void JsonDeserializer::OnMapEntryBegin() {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "object");
  }
  Push(val);
}

void JsonDeserializer::OnMapEntryEnd() { Pop(); }

void JsonDeserializer::OnMapKeyBegin() { SetTag("key"); }

void JsonDeserializer::OnMapValueBegin() { SetTag("value"); }

void JsonDeserializer::OnObjectBegin() {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "object");
  }
  Push(val);
}

void JsonDeserializer::OnObjectEnd() { Pop(); }

void JsonDeserializer::OnPairBegin() {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "object");
  }
  Push(val);
}

void JsonDeserializer::OnPairKeyBegin() { SetTag("key"); }

void JsonDeserializer::OnPairValueBegin() { SetTag("value"); }

void JsonDeserializer::OnPairEnd() { Pop(); }

//===------------------------------------------------------------------------===
// Primitive types
//===------------------------------------------------------------------------===

auto JsonDeserializer::ReadBool() -> bool {
  auto *val = GetNextValue();
  if (!yyjson_is_bool(val)) {
    ThrowTypeError(val, "bool");
  }
  return yyjson_get_bool(val);
}

auto JsonDeserializer::ReadSignedInt8() -> i8 {
  auto *val = GetNextValue();
  if (!yyjson_is_sint(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<i8>(yyjson_get_sint(val));
}

auto JsonDeserializer::ReadUnsignedInt8() -> u8 {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<u8>(yyjson_get_uint(val));
}

auto JsonDeserializer::ReadSignedInt16() -> i16 {
  auto *val = GetNextValue();
  if (!yyjson_is_sint(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<i16>(yyjson_get_sint(val));
}

auto JsonDeserializer::ReadUnsignedInt16() -> u16 {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<u16>(yyjson_get_uint(val));
}

auto JsonDeserializer::ReadSignedInt32() -> i32 {
  auto *val = GetNextValue();
  if (!yyjson_is_sint(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<i32>(yyjson_get_sint(val));
}

auto JsonDeserializer::ReadUnsignedInt32() -> u32 {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<u32>(yyjson_get_uint(val));
}

auto JsonDeserializer::ReadSignedInt64() -> i64 {
  auto *val = GetNextValue();
  if (!yyjson_is_sint(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<i64>(yyjson_get_sint(val));
}

auto JsonDeserializer::ReadUnsignedInt64() -> u64 {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<u64>(yyjson_get_uint(val));
}

auto JsonDeserializer::ReadFloat() -> float {
  auto *val = GetNextValue();
  if (!yyjson_is_real(val)) {
    ThrowTypeError(val, "float");
  }
  return static_cast<float>(yyjson_get_real(val));
}

auto JsonDeserializer::ReadDouble() -> double {
  auto *val = GetNextValue();
  if (!yyjson_is_real(val)) {
    ThrowTypeError(val, "double");
  }
  return yyjson_get_real(val);
}

auto JsonDeserializer::ReadString() -> string {
  auto *val = GetNextValue();
  if (!yyjson_is_str(val)) {
    ThrowTypeError(val, "string");
  }
  return {yyjson_get_str(val), yyjson_get_len(val)};
}

} // namespace saturn
