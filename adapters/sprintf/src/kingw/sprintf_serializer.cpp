#include "kingw/sprintf_serializer.hpp"

#include <cstdio>


namespace kingw {

SPrintfSerializer::SPrintfSerializationException::SPrintfSerializationException(const char * message)
    : ser::SerializationException(message) { }

SPrintfSerializer::SPrintfSerializer(char * buffer, std::size_t len, bool human_readable)
    : buffer(buffer), len(len), human_readable(human_readable) { }

bool SPrintfSerializer::is_human_readable() const {
    return human_readable;
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
    int ret = std::snprintf(buffer, len, "%ld", value);
    if (ret >= 0) {
        if (ret < len) {
            ret += 1;
        }
        buffer += ret;
        len -= ret;
    } else {
        // TODO
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
    int ret = std::snprintf(buffer, len, "%lu", value);
    if (ret >= 0) {
        if (ret < len) {
            ret += 1;
        }
        buffer += ret;
        len -= ret;
    } else {
        // TODO
    }
}
void SPrintfSerializer::serialize_f32(float value) {
    serialize_f64(value);
}
void SPrintfSerializer::serialize_f64(double value) {
    int ret = std::snprintf(buffer, len, "%lf", value);
    if (ret >= 0) {
        if (ret < len) {
            ret += 1;
        }
        buffer += ret;
        len -= ret;
    } else {
        // TODO
    }
}
void SPrintfSerializer::serialize_char(char value) {
    int ret = std::snprintf(buffer, len, "%c", value);
    if (ret >= 0) {
        if (ret < len) {
            ret += 1;
        }
        buffer += ret;
        len -= ret;
    } else {
        // TODO
    }
}
void SPrintfSerializer::serialize_c_str(const char * value) {
    int ret = std::snprintf(buffer, len, "%s", value);
    if (ret >= 0) {
        if (ret < len) {
            ret += 1;
        }
        buffer += ret;
        len -= ret;
    } else {
        // TODO
    }
}
void SPrintfSerializer::serialize_string(const std::string & value) {
    int ret = std::snprintf(buffer, len, "%s", value.c_str());
    if (ret >= 0) {
        if (ret < len) {
            ret += 1;
        }
        buffer += ret;
        len -= ret;
    } else {
        // TODO
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

void SPrintfSerializer::struct_begin(const char* name, std::size_t len) {
    // Serialize the number of fields in the struct.
    // If len is not provided, then throw an exception.
    if (len == UNKNOWN_LENGTH) {
        throw SPrintfSerializationException("SPrintfSerializer::serialize_struct() requires length parameter");
    } else {
        ser::serialize(*this, len);
    }
}

void SPrintfSerializer::struct_serialize_field(const char * name, const ser::Serialize & field) {
    serialize_c_str(name);
    field.serialize(*this);
}

void SPrintfSerializer::struct_skip_field(const char * name) {
    // No-op
}

void SPrintfSerializer::struct_end() {
    // No-op
}

}  // namespace kingw
