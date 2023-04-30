cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(json 
    URL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz")
FetchContent_MakeAvailable(json)

add_library(kingw_dynamic_serde_json)
target_include_directories(kingw_dynamic_serde_json
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_dynamic_serde_json
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/json_serializer.cpp")
target_link_libraries(kingw_dynamic_serde_json
    PUBLIC
        kingw::dynamic_serde
        kingw::dynamic_serde_ostream
        nlohmann_json::nlohmann_json)

add_library(kingw::dynamic_serde_json ALIAS kingw_dynamic_serde_json)
