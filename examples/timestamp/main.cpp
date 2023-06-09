#include <iostream>

#include "kingw/serde_sprintf.hpp"
using namespace kingw;

#include "timestamp.hpp"


int main() {
    const std::size_t buffer_len = 32;
    char buffer[buffer_len] = {};
    auto millis = [](auto time) {
        return std::chrono::time_point_cast<std::chrono::milliseconds>(time)
            .time_since_epoch().count();
    };

    // Create a timestamp with the current time.
    Timestamp timestamp{ std::chrono::system_clock::now() };
    Timestamp deserialized{};
    std::cout << "Current time (milliseconds since 1970-01-01 UTC): " << millis(timestamp.value) << "\n\n";
    
    // Serialize/Deserialize the timestamp as a string formatted to "yyyy-mm-ddThh:mm:ss.fffZ".
    bool human_readable = true;
    serde_sprintf::to_buffer(timestamp, std::begin(buffer), std::end(buffer), human_readable);
    std::cout << "Timestamp serialized in human-readable format (yyyy-mm-ddThh:mm:ss.fffZ): " << buffer << "\n";
    serde_sprintf::from_string(deserialized, buffer, human_readable);
    std::cout << "Deserialized time in milliseconds: " << millis(deserialized.value) << "\n\n";

    // Serialize/Deserialize the timestamp as the number of milliseconds since 1970-01-01 UTC.
    human_readable = false;
    serde_sprintf::to_buffer(timestamp, std::begin(buffer), std::end(buffer), human_readable);
    std::cout << "Timestamp serialized in non-human-readable format (milliseconds since 1970-01-01 UTC): " << buffer << "\n";
    serde_sprintf::from_string(deserialized, buffer, human_readable);
    std::cout << "Deserialized time in milliseconds: " << millis(deserialized.value) << "\n\n";

    return 0;
}
