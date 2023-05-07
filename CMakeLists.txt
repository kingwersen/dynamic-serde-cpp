cmake_minimum_required(VERSION 3.16)

project(dynamic_serde)


# Default enable everything if the user is only building this repo.
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(KINGW_SERDE_BUILD_EXAMPLES   "Build Examples" ON)
    option(KINGW_SERDE_BUILD_TESTS      "Build Tests and GMock support" ON)
else()
    option(KINGW_SERDE_BUILD_EXAMPLES   "Build Examples" OFF)
    option(KINGW_SERDE_BUILD_TESTS      "Build Tests and GMock support" OFF)
endif()

# Additional configuration if testing is enabled.
if (KINGW_SERDE_BUILD_TESTS)
    # Enable CTest
    enable_testing()

    # Generate a Test Coverage report
    if (UNIX AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(--coverage)
        add_link_options(--coverage)
    endif()
endif()


# Build this library.
add_library(kingw_dynamic_serde)
target_include_directories(kingw_dynamic_serde
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(kingw_dynamic_serde
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/de/deserializer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kingw/ser/serializer.cpp")

add_library(kingw::dynamic_serde ALIAS kingw_dynamic_serde)


# Build adapters.
# TODO: Move to other repositories.
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/adapters")

# Build examples.
# Run CMake with -D KINGW_SERDE_BUILD_EXAMPLES=ON
if (KINGW_SERDE_BUILD_EXAMPLES)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif()

# Build tests and GMock support.
# Run CMake with -D KINGW_SERDE_BUILD_TESTS=ON
if (KINGW_SERDE_BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
endif()
