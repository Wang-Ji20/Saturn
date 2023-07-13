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

void JsonSerializer::onSerializeBegin() {
  buffer.push_back('{');
}

void JsonSerializer::onSerializeEnd() {
  buffer.push_back('}');
}

void JsonSerializer::OnObjectBegin() {
  // do nothing
}

void JsonSerializer::OnObjectEnd() {
  // do nothing
}

//===------------------------------------------------===
// primitive types
//===------------------------------------------------===

void JsonSerializer::WriteValue(const string &value) {
  buffer.push_back('\"');
  buffer.insert(buffer.end(), value.begin(), value.end());
  buffer.push_back('\"');
}

void JsonSerializer::WriteValue(const char *value) {
  buffer.push_back('\"');
  buffer.insert(buffer.end(), value, value + strlen(value));
  buffer.push_back('\"');
}

void JsonSerializer::WriteNull() {
  WriteValue("null");
}

void JsonSerializer::WriteValue(const bool value) {
  WriteValue(value ? "true" : "false");
}

void JsonSerializer::WriteValue(const int8_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const uint8_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const int16_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const uint16_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const int32_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const uint32_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const int64_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const uint64_t value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const float value) {
  WriteValue(std::to_string(value));
}

void JsonSerializer::WriteValue(const double value) {
  WriteValue(std::to_string(value));
}

} // namespace saturn
