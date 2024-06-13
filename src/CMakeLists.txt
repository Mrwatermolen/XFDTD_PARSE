add_subdirectory(material_map)
add_subdirectory(shape_map)

add_library(xfdtd_parse ${XFDTD_PARSE_ALL_OBJECTS})

set(XFDTD_PARSE_LIBRARIES
    xfdtd_parse_material_map
    xfdtd_parse_shape_map
)

target_link_libraries(xfdtd_parse PUBLIC ${XFDTD_PARSE_LIBRARIES})
