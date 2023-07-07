add_subdirectory(allocator)

add_library(
    storage
    OBJECT
    allocator.cc
    block.cc
    file_buffer.cc
    buffer_pool.cc
)

set(
    STORAGE_LIBS
    storage_allocator
)

target_link_libraries(
    storage
    ${STORAGE_LIBS}
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:storage>
    PARENT_SCOPE
)
