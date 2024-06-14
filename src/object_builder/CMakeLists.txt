aux_source_directory(. XFDTD_PARSE_OBJECT_BUILDER_SRC)
add_library(xfdtd_parse_object_builder OBJECT ${XFDTD_PARSE_OBJECT_BUILDER_SRC})
target_link_libraries(xfdtd_parse_object_builder PUBLIC xfdtd::xfdtd_core)
set(${XFDTD_PARSE_ALL_OBJECTS} ${XFDTD_PARSE_ALL_OBJECTS} $<TARGET_OBJECTS:xfdtd_parse_object_builder> PARENT_SCOPE)
