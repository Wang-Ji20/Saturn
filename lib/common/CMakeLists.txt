add_library(
    Common
    OBJECT
    assert.cc
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:Common>
    PARENT_SCOPE
)
