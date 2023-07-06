add_library(
    storage
    OBJECT
    allocator.cc
    block.cc
    file_buffer.cc
    buffer_pool.cc
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:storage>
    PARENT_SCOPE
)
