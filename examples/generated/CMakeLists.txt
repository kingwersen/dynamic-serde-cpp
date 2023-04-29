cmake_minimum_required(VERSION 3.16)

add_library(kingw_dynamic_serde_ex_gen)
target_include_directories(kingw_dynamic_serde_ex_gen
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_dynamic_serde_ex_gen
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/structs.cpp")
target_link_libraries(kingw_dynamic_serde_ex_gen
    PUBLIC
        kingw::dynamic_serde)

add_library(kingw::dynamic_serde_ex_gen ALIAS kingw_dynamic_serde_ex_gen)
