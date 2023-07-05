add_library(
    port
    OBJECT
    file_handle.cc
    file_system.cc
    unix_file_handle.cc
    unix_file_system.cc
    windows_file_system.cc
)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:port>
    PARENT_SCOPE
)
