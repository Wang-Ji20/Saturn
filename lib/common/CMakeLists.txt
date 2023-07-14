add_subdirectory(serializer)

# I am hurt by this....
# parent_scope is not propagated to grandparents
set(
  ALL_OBJECT_FILES
  ${ALL_OBJECT_FILES}
  PARENT_SCOPE
)
