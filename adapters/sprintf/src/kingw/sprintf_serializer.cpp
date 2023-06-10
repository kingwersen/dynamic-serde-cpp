#include "kingw/sprintf_serializer.hpp"

#include <cstdio>
#include <cstring>


namespace kingw {
namespace serde_sprintf {

SPrintfSerializer::SPrintfSerializationException::SPrintfSerializationException(serde::string_view message)
    : ser::SerializationException(message) { }

SPrintfSerializer::SPrintfSerializer(char* buffer_begin, char* buffer_end, bool human_readable)
    : buffer({ buffer_begin, buffer_end }), last_end_(buffer_begin), human_readable(human_readable) { }

bool SPrintfSerializer::is_human_readable() const {
    return human_readable;
}

char* SPrintfSerializer::last_end() const {
    return last_end_;
}

void SPrintfSerializer::serialize_bool(bool value) {
    serialize_i64(value);
}
void SPrintfSerializer::serialize_i8(std::int8_t value) {
    serialize_i64(value);
}
void SPrintfSerializer::serialize_i16(std::int16_t value) {
    serialize_i64(value);
}
void SPrintfSerializer::serialize_i32(std::int32_t value) {
    serialize_i64(value);
}
void SPrintfSerializer::serialize_i64(std::int64_t value) {
    int ret = std::snprintf(buffer.begin, buffer.size(), "%ld", value);
    if (ret >= 0) {
        advance(ret);
    } else {
        throw SPrintfSerializationException("failed to serialize integer");
    }
}
void SPrintfSerializer::serialize_u8(std::uint8_t value) {
    serialize_u64(value);
}
void SPrintfSerializer::serialize_u16(std::uint16_t value) {
    serialize_u64(value);
}
void SPrintfSerializer::serialize_u32(std::uint32_t value) {
    serialize_u64(value);
}
void SPrintfSerializer::serialize_u64(std::uint64_t value) {
    int ret = std::snprintf(buffer.begin, buffer.size(), "%lu", value);
    if (ret >= 0) {
        advance(ret);
    } else {
        throw SPrintfSerializationException("failed to serialize unsigned integer");
    }
}
void SPrintfSerializer::serialize_f32(float value) {
    serialize_f64(value);
}
void SPrintfSerializer::serialize_f64(double value) {
    int ret = std::snprintf(buffer.begin, buffer.size(), "%lf", value);
    if (ret >= 0) {
        advance(ret);
    } else {
        throw SPrintfSerializationException("failed to serialize floating point type");
    }
}
void SPrintfSerializer::serialize_char(char value) {
    int ret = std::snprintf(buffer.begin, buffer.size(), "%c", value);
    if (ret >= 0) {
        advance(ret);
    } else {
        throw SPrintfSerializationException("failed to serialize character");
    }
}
void SPrintfSerializer::serialize_string(serde::string_view value) {
    if (value.size() <= buffer.size()) {
        std::copy(value.begin(), value.end(), buffer.begin);
        advance(value.size());
    } else {
        throw SPrintfSerializationException("failed to serialize string - too long");
    }
}


///
/// Sequences 
///

void SPrintfSerializer::seq_begin(std::size_t len) {
    // Serialize the number of elements in the sequence.
    // If len is not provided, then throw an exception.
    if (len == UNKNOWN_LENGTH) {
        throw SPrintfSerializationException("SPrintfSerializer::serialize_seq() requires length parameter");
    } else {
        ser::serialize(*this, len);
    }
}

void SPrintfSerializer::seq_serialize_element(const ser::Serialize & element) {
    element.serialize(*this);
}

void SPrintfSerializer::seq_end() {
    // No-op
}


///
/// Maps 
///

void SPrintfSerializer::map_begin(std::size_t len) {
    // Serialize the number of entries in the map.
    // If len is not provided, then throw an exception.
    if (len == UNKNOWN_LENGTH) {
        throw SPrintfSerializationException("SPrintfSerializer::serialize_map() requires length parameter");
    } else {
        ser::serialize(*this, len);
    }
}

void SPrintfSerializer::map_serialize_key(const ser::Serialize & key) {
    key.serialize(*this);
}

void SPrintfSerializer::map_serialize_value(const ser::Serialize & value) {
    value.serialize(*this);
}

void SPrintfSerializer::map_serialize_entry(const ser::Serialize & key, const ser::Serialize & value) {
    map_serialize_key(key);
    map_serialize_value(value);
}

void SPrintfSerializer::map_end() {
    // No-op
}


///
/// Structs 
///

void SPrintfSerializer::struct_begin(serde::string_view name, std::size_t len) {
    // Serialize the number of fields in the struct.
    // If len is not provided, then throw an exception.
    if (len == UNKNOWN_LENGTH) {
        throw SPrintfSerializationException("SPrintfSerializer::serialize_struct() requires length parameter");
    } else {
        ser::serialize(*this, len);
    }
}

void SPrintfSerializer::struct_serialize_field(serde::string_view name, const ser::Serialize & field) {
    serialize_string(name);
    field.serialize(*this);
}

void SPrintfSerializer::struct_skip_field(serde::string_view name) {
    // No-op
}

void SPrintfSerializer::struct_end() {
    // No-op
}

std::size_t SPrintfSerializer::Buffer::size() const {
    return end - begin;
}

void SPrintfSerializer::advance(std::size_t distance) {
    auto size_ = buffer.size();
    if (distance < size_) {
        // Always delimit sprintf() calls with '\0'.
        last_end_ = buffer.begin + distance;
        buffer.begin[distance] = '\0';
        buffer.begin += distance + 1;
    } else {
        if (distance > size_) {
            // Sanity check against OOB.
            distance = size_;
        }
        last_end_ = buffer.begin + distance;
        buffer.begin += distance;
    }
}

}  // namespace serde_sprintf
}  // namespace kingw
