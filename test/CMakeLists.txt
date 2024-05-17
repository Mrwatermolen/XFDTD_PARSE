add_executable(test_parse test_parse.cpp)
target_link_libraries(test_parse xfdtd_parse)
add_test(NAME test_parse COMMAND test_parse WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
