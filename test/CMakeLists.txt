cmake_minimum_required(VERSION 3.16)


# Download GoogleTest with FetchContent. See documentation for configuration.
# You can disable or override the download without changing this repository.
# https://cmake.org/cmake/help/latest/module/FetchContent.html
include(FetchContent)
FetchContent_Declare(googletest
    URL         "https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz"
    URL_HASH    "SHA256=81964FE578E9BD7C94DFDB09C8E4D6E6759E19967E397DBEA48D1C10E45D0DF2")
FetchContent_MakeAvailable(googletest)


# GMock support.
add_library(kingw_dynamic_serde_mock INTERFACE)
target_include_directories(kingw_dynamic_serde_mock
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(kingw_dynamic_serde_mock
    INTERFACE
        kingw::dynamic_serde
        gmock)

add_library(kingw::dynamic_serde_mock ALIAS kingw_dynamic_serde_mock)


# Build test executable.
add_executable(kingw_dynamic_serde_test)
target_include_directories(kingw_dynamic_serde_test
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(kingw_dynamic_serde_test
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/unit/kingw/de/test_deserialize.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/unit/kingw/de/test_integral_visitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/unit/kingw/ser/test_serialize.cpp")
target_link_libraries(kingw_dynamic_serde_test
    PRIVATE
        kingw::dynamic_serde
        kingw::dynamic_serde_mock
        gmock_main)


# Add this test executable to CTest.
add_test(
    NAME kingw_dynamic_serde_test
    COMMAND $<TARGET_FILE:kingw_dynamic_serde_test>)
