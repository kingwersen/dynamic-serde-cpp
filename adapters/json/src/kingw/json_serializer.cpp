#include "kingw/json_serializer.hpp"

#include "kingw/ostream_serializer.hpp"


namespace kingw {

JsonSerializer::JsonSerializationException::JsonSerializationException(const char* message)
    : ser::SerializationException(message) { }

JsonSerializer::JsonSerializer() {
    json_stack.push({});
}

bool JsonSerializer::is_human_readable() const {
    return true;
}

std::string JsonSerializer::dump() const {
    if (json_stack.size() == 1) {
        return json_stack.top().dump();
    } else {
        // Logic error - this shouldn't occur if the user is using
        // serialize_seq(), _map(), _struct(), etc.
        throw JsonSerializationException("JsonSerializer internal data structure corrupted during serialization");
    }
}

void JsonSerializer::serialize_bool(bool value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_i8(std::int8_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_i16(std::int16_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_i32(std::int32_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_i64(std::int64_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_u8(std::uint8_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_u16(std::uint16_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_u32(std::uint32_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_u64(std::uint64_t value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_f32(float value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_f64(double value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_char(char value) {
    json_stack.top() = value;
}
void JsonSerializer::serialize_string(const char * begin, const char * end) {
    json_stack.top() = std::string(begin, end);
}


///
/// Sequences 
///

void JsonSerializer::seq_begin(std::size_t len) {
    // No-op. Assume previous serialize calls already
    // gave us a nlohmann::json object to work with
    // at the top of the stack.
}

void JsonSerializer::seq_serialize_element(const ser::Serialize & accessor) {
    json_stack.push({});
    accessor.serialize(*this);
    if (json_stack.size() < 2) {
        // Logic error - this shouldn't occur if the user is using
        // serialize_seq(), _map(), _struct(), etc.
        throw JsonSerializationException("JsonSerializer internal data structure corrupted during serialization");
    }
    nlohmann::json json = std::move(json_stack.top());
    json_stack.pop();
    json_stack.top() += std::move(json);
}

void JsonSerializer::seq_end() {
    // No-op
}


///
/// Maps 
///

void JsonSerializer::map_begin(std::size_t len) {
    // No-op. Assume previous serialize calls already
    // gave us a nlohmann::json object to work with
    // at the top of the stack.
}

void JsonSerializer::map_serialize_key(const ser::Serialize & accessor) {
    if (!accessor.traits().is_string) {
        throw JsonSerializationException("JsonSerializer map key is not a string");
    }

    // Convert the key into a string.
    current_map_key = kingw::OStreamSerializer::to_string(accessor);
}

void JsonSerializer::map_serialize_value(const ser::Serialize & accessor) {
    // Add a new nlohmann::json object to the top of the stack.
    // accessor.serialize() will update it recursively.
    std::string name = std::move(current_map_key);
    json_stack.push({});
    accessor.serialize(*this);
    if (json_stack.size() < 2) {
        // Logic error - this shouldn't occur if the user is using
        // serialize_seq(), _map(), _struct(), etc.
        throw JsonSerializationException("JsonSerializer internal data structure corrupted during serialization");
    }
    nlohmann::json value = std::move(json_stack.top());
    json_stack.pop();
    json_stack.top()[std::move(name)] = std::move(value);
}

void JsonSerializer::map_serialize_entry(const ser::Serialize & key, const ser::Serialize & value) {
    map_serialize_key(key);
    map_serialize_value(value);
}

void JsonSerializer::map_end() {
    // No-op
}


///
/// Structs 
///

void JsonSerializer::struct_begin(const char* name, std::size_t len) {
    // No-op. Assume previous serialize calls already 
    // gave us a nlohmann::json object to work with
    // at the top of the stack.
}

void JsonSerializer::struct_serialize_field(const char * name, const ser::Serialize & accessor) {
    // Add a new nlohmann::json object to the top of the stack.
    // accessor.serialize() will update it recursively.
    json_stack.push({});
    accessor.serialize(*this);
    if (json_stack.size() < 2) {
        // Logic error - this shouldn't occur if the user is using
        // serialize_seq(), _map(), _struct(), etc.
        throw JsonSerializationException("JsonSerializer internal data structure corrupted during serialization");
    }
    nlohmann::json field = std::move(json_stack.top());
    json_stack.pop();
    json_stack.top()[name] = std::move(field);
}

void JsonSerializer::struct_skip_field(const char * name) {
    // No-op
}

void JsonSerializer::struct_end() {
    // No-op
}

}  // namespace kingw
