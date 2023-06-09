cmake_minimum_required(VERSION 3.16)


add_executable(example_timestamp)
target_include_directories(example_timestamp
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(example_timestamp
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/timestamp.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(example_timestamp
    PRIVATE
        kingw::dynamic_serde
        kingw::dynamic_serde_sprintf)
