cmake_minimum_required(VERSION 3.16)

project(dynamic_serde)


add_library(kingw_dynamic_serde)
target_include_directories(kingw_dynamic_serde
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_dynamic_serde
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/serde/serialize.cpp")
target_link_options(kingw_dynamic_serde
    PUBLIC
        -u _serialize)

add_library(kingw::dynamic_serde ALIAS kingw_dynamic_serde)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/adapters")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
