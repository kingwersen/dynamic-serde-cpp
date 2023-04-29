cmake_minimum_required(VERSION 3.16)

add_library(kingw_dynamic_serde_xml)
target_include_directories(kingw_dynamic_serde_xml
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_dynamic_serde_xml
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/xml_serializer.cpp")
target_link_libraries(kingw_dynamic_serde_xml
    PUBLIC
        kingw::dynamic_serde)

add_library(kingw::dynamic_serde_xml ALIAS kingw_dynamic_serde_xml)
