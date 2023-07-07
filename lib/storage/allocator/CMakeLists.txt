add_library(
    storage_allocator
    OBJECT
    arena_allocator.cc
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:storage_allocator>
    PARENT_SCOPE
)
