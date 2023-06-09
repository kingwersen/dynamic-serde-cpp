cmake_minimum_required(VERSION 3.16)

add_executable(example_multiple_libs_exe1)
target_include_directories(example_multiple_libs_exe1
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(example_multiple_libs_exe1
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(example_multiple_libs_exe1
    PUBLIC
        kingw::dynamic_serde_sprintf
        kingw::dynamic_serde_json
        kingw::dynamic_serde_xml
        example_multiple_libs_lib1)
