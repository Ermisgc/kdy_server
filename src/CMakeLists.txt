file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*cpp)
add_executable(test_server ${SRC_FILES})

target_link_libraries(test_server ${ALL_LIBRARIES})