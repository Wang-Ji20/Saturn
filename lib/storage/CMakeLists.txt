add_subdirectory(allocator)

add_library(
    storage
    OBJECT
    allocator.cc
    block.cc
    file_buffer.cc
    buffer_pool.cc
    block_handle.cc
    block_manager.cc
    single_file_block_manager.cc
    storage_manager.cc
    single_file_storage_manager.cc
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:storage>
    PARENT_SCOPE
)
