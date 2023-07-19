add_library(
  common_serializer
  OBJECT
  serializer.cc
  json_serializer.cc
  json_deserializer.cc
  binary_serializer.cc
  binary_deserializer.cc
)

set(
  ALL_OBJECT_FILES
  ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:common_serializer>
  PARENT_SCOPE
)
