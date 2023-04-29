cmake_minimum_required(VERSION 3.16)

add_library(kingw_ex_generated1)
target_include_directories(kingw_ex_generated1
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_ex_generated1
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/structs1.cpp")
target_link_libraries(kingw_ex_generated1
    PUBLIC
        kingw::dynamic_serde
        kingw::ex_generated2)

add_library(kingw::ex_generated1 ALIAS kingw_ex_generated1)
