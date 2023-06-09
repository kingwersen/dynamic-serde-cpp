#include "timestamp.hpp"
#include <iostream>

#include "kingw/serde/derive.hpp"
using namespace kingw;


/// @brief Serialize a Timestamp
///
/// If using a human readable serializer, outputs "yyyy-mm-ddThh:mm:ss.fffZ"
/// Else, outputs milliseconds since 1970-01-01 UTC.
///
/// @param serializer Serializer to insert into
/// @param timestamp Item to serialize
template <>
void ser::serialize<Timestamp>(ser::Serializer & serializer, const Timestamp & timestamp) {
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
        (timestamp.value.time_since_epoch()).count();
    if (serializer.is_human_readable()) {
        // Serialize the timestamp as a string formatted to "yyyy-mm-ddThh:mm:ss.fffZ".
        // Use std::strftime to print up to seconds.
        std::time_t time = std::chrono::system_clock::to_time_t(timestamp.value);
        std::tm tp{};
        gmtime_r(&time, &tp);
        const std::size_t buffer_len = 32;
        char buffer[buffer_len] = {};
        std::size_t len = std::strftime(buffer, buffer_len, "%FT%T", &tp);

        // Append milliseconds manually since std::strftime doesn't support it.
        std::snprintf(buffer + len, buffer_len - len, ".%ldZ", milliseconds % 1000);
        serializer.serialize_c_str(buffer);
    } else {
        // Serialize the timestamp as the number of milliseconds since 1970-01-01 UTC.
        serializer.serialize_i64(milliseconds);
    }
}



namespace {
    /// @brief Visitor that deserializes Timestamp from strings
    ///
    /// Deserializes timestamps in the format "yyyy-mm-ddThh:mm:ss(.f*)Z".
    /// Supports up to nanoseconds accuracy (optional).
    ///
    /// This is a half-baked implementation I wrote myself. Not really tested.
    /// Written quickly for the sake of demonstrating Serde.
    ///
    /// You can use it of course, but you might prefer to use a more stable
    /// implementation like: https://github.com/HowardHinnant/date/tree/master
    class TimestampStringVisitor : public de::Visitor {
    public:
        /// @brief Output value to modify
        Timestamp & timestamp;

        /// @brief Visitor Constructor
        /// @param timestamp Output value to modify
        explicit TimestampStringVisitor(Timestamp & timestamp)
            : timestamp(timestamp) {}

        /// @brief Human-readable hint at what the visitor is expecting
        /// @return A string literal
        const char* expecting() const override {
            return "a timestamp";
        }

        /// @brief Parse a calendar timestamp from a string
        ///
        /// Deserializes timestamps in the format "yyyy-mm-ddThh:mm:ss(.f*)Z".
        /// Supports up to nanoseconds accuracy (optional).
        ///
        /// @param buffer Pointer to character array
        /// @param len Length of character array
        void visit_c_str(const char* buffer, std::size_t len = -1) override {
            if (len == -1) {
                len = std::strlen(buffer);
            }
            // Verify the end of the string has a Z.
            // Now we can use 'Z' as the sentinel instead of len or '\0'.
            if (len > 0 && buffer[len-1] != 'Z') {
                throw de::DeserializationException("failed to parse timestamp from string");
            }

            // Deserialize the timestamp from a string formatted to "yyyy-mm-ddThh:mm:ss(.f*)Z".
            // Use strptime() to parse "yyyy-mm-ddThh:mm:ss".
            // strptime() is guaranteed to not reach len because it doesn't parse 'Z'.
            std::tm tp{};
            const char* end = strptime(buffer, "%FT%T", &tp);
            if (*end == 'Z') {
                // Timestamp does not have seconds in decimal format.
                timestamp.value = std::chrono::system_clock::from_time_t(timegm(&tp));
            } else if (*end == '.') {
                // Timestamp has seconds in decimal format.
                buffer = end + 1;  // Move buffer to the beginning of the decimal value.

                // Parse seconds decimal manually since strptime() can only handle seconds.
                const std::size_t nanos_len = 9;  // Up to nanoseconds accuracy
                char decimal[nanos_len + 1] = {};
                std::size_t i = 0;
                while (i < nanos_len && buffer[i] >= '0' && buffer[i] <= '9') { decimal[i] = buffer[i]; ++i; }
                if (buffer[i] != 'Z') {
                    throw de::DeserializationException("failed to parse timestamp from string");
                }
                while (i < nanos_len) { decimal[i++] = '0'; }

                timestamp.value = std::chrono::system_clock::from_time_t(timegm(&tp));
                timestamp.value += std::chrono::nanoseconds(std::strtoull(decimal, nullptr, 10));
            } else {
                throw de::DeserializationException("failed to parse timestamp from string");
            }
        }

        /// @brief Parse a calendar timestamp from a string
        /// @see visit_c_str() for implementation
        /// @param value String to parse
        void visit_string(const std::string & value) override {
            visit_c_str(value.c_str(), value.size());
        }
    };
}

/// @brief Deserialize a Timestamp
///
/// If using a human readable deserializer, expects "yyyy-mm-ddThh:mm:ss(.f*)Z"
/// Else, expects milliseconds since 1970-01-01 UTC.
///
/// @param deserializer Deserializer to extract from
/// @param timestamp Output location
template <>
void de::deserialize<Timestamp>(de::Deserializer & deserializer, Timestamp & timestamp) {
    if (deserializer.is_human_readable()) {
        // Deserialize the timestamp from a string formatted to "yyyy-mm-ddThh:mm:ss(.*f)Z".
        // See TimestampStringVisitor.
        TimestampStringVisitor visitor(timestamp);
        deserializer.deserialize_string(visitor);
    } else {
        // Deserialize the timestamp from an integer number of milliseconds since 1970-01-01 UTC.
        std::int64_t milliseconds{};
        de::deserialize(deserializer, milliseconds);
        timestamp.value = std::chrono::system_clock::from_time_t(std::time_t{0})
            + std::chrono::milliseconds(milliseconds);
    }
}
