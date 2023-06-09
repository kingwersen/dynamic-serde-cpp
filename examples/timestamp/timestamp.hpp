#pragma once

#include <chrono>


/// @brief Custom timestamp Serde implementation
///
/// This class defines a strategy for serializing and deserializing
/// a timestamp. This particular implementation will be serialized
/// and deserialized in the format of "yyyy-mm-ddThh:mm:ss.fffZ"
/// or milliseconds since 1970-01-01 UTC, see timestamp.cpp.
///
/// The reason we create a whole new class instead of implementing
/// serde for std::chrono::system_clock::time_point is to prevent
/// conflicting implementations. For example, one library may choose
/// to serialize a timestamp in milliseconds and another may
/// choose to serialize in nanoseconds. If serialization is defined
/// for std::chrono::system_clock::time_point, then there is no way
/// to select which implementation to use.
///
/// By wrapping in a custom class, we can now choose serde implementations
/// by selecting the corresponding wrapper class.
struct Timestamp {
    /// @brief Timestamp value
    std::chrono::system_clock::time_point value;
};
