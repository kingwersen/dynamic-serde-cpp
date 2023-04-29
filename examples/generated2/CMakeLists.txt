cmake_minimum_required(VERSION 3.16)

add_library(kingw_ex_generated2)
target_include_directories(kingw_ex_generated2
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_ex_generated2
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/structs2.cpp")
target_link_libraries(kingw_ex_generated2
    PUBLIC
        kingw::dynamic_serde)

add_library(kingw::ex_generated2 ALIAS kingw_ex_generated2)
