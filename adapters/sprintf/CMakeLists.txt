cmake_minimum_required(VERSION 3.16)

add_library(kingw_dynamic_serde_sprintf)
target_include_directories(kingw_dynamic_serde_sprintf
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_dynamic_serde_sprintf
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/sprintf_serializer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/sprintf_deserializer.cpp")
target_link_libraries(kingw_dynamic_serde_sprintf
    PUBLIC
        kingw::dynamic_serde)

add_library(kingw::dynamic_serde_sprintf ALIAS kingw_dynamic_serde_sprintf)
