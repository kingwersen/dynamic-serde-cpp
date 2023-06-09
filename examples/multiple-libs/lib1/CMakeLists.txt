cmake_minimum_required(VERSION 3.16)

add_library(example_multiple_libs_lib1)
target_include_directories(example_multiple_libs_lib1
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(example_multiple_libs_lib1
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/structs1.cpp")
target_link_libraries(example_multiple_libs_lib1
    PUBLIC
        kingw::dynamic_serde
        example_multiple_libs_lib2)
