add_library(
    storage
    OBJECT
    allocator.cc
    block.cc
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:storage>
    PARENT_SCOPE
)
