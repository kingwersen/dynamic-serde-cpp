cmake_minimum_required(VERSION 3.16)

add_executable(kingw_dynamic_serde_example1)
target_include_directories(kingw_dynamic_serde_example1
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(kingw_dynamic_serde_example1
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/example1.cpp")
target_link_libraries(kingw_dynamic_serde_example1
    PUBLIC
        kingw::dynamic_serde_json
        kingw::dynamic_serde_xml
        kingw::dynamic_serde_ex_gen)
