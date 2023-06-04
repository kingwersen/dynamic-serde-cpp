#include "kingw/sprintf_deserializer.hpp"

#include "kingw/de/accessor.hpp"


namespace kingw {

SPrintfDeserializer::SPrintfDeserializationException::SPrintfDeserializationException(const char * message)
    : de::DeserializationException(message) { }

SPrintfDeserializer::SPrintfDeserializer(const char * buffer, std::size_t len, bool human_readable)
    : buffer(buffer), buffer_end(buffer + len), human_readable(human_readable) { }

bool SPrintfDeserializer::is_human_readable() const {
    return human_readable;
}

// Basic Types
void SPrintfDeserializer::deserialize_any(de::Visitor & visitor) {
    throw SPrintfDeserializationException("deserialize_any() not implemented");
}
void SPrintfDeserializer::deserialize_bool(de::Visitor & visitor) {
    DelimitedString next = next_delimited_string();
    if (next.len == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else if (next.len != 1 || *next.buffer != '0' || *next.buffer != '1') {
        throw SPrintfDeserializationException("element is not a boolean");
    } else {
        visitor.visit_bool(*next.buffer == '1');
    }
}
void SPrintfDeserializer::deserialize_i8(de::Visitor & visitor) {
    deserialize_i64(visitor);
}
void SPrintfDeserializer::deserialize_i16(de::Visitor & visitor) {
    deserialize_i64(visitor);
}
void SPrintfDeserializer::deserialize_i32(de::Visitor & visitor) {
    deserialize_i64(visitor);
}
void SPrintfDeserializer::deserialize_i64(de::Visitor & visitor) {
    DelimitedString next = next_delimited_string();
    if (next.len == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        char* end{};
        std::int64_t value = std::strtol(next.buffer, &end, 10);
        if (end == (next.buffer + next.len)) {
            visitor.visit_i64(value);
        } else {
            throw SPrintfDeserializationException("element is not an integer or is too long");
        }
    }
}
void SPrintfDeserializer::deserialize_u8(de::Visitor & visitor) {
    deserialize_u64(visitor);
}
void SPrintfDeserializer::deserialize_u16(de::Visitor & visitor) {
    deserialize_u64(visitor);
}
void SPrintfDeserializer::deserialize_u32(de::Visitor & visitor) {
    deserialize_u64(visitor);
}
void SPrintfDeserializer::deserialize_u64(de::Visitor & visitor) {
    DelimitedString next = next_delimited_string();
    if (next.len == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        char* end{};
        std::uint64_t value = std::strtoul(next.buffer, &end, 10);
        if (end == (next.buffer + next.len)) {
            visitor.visit_u64(value);
        } else {
            throw SPrintfDeserializationException("element is not an unsigned integer or is too long");
        }
    }
}
void SPrintfDeserializer::deserialize_f32(de::Visitor & visitor) {
    deserialize_f64(visitor);
}
void SPrintfDeserializer::deserialize_f64(de::Visitor & visitor) {
    DelimitedString next = next_delimited_string();
    if (next.len == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        char* end{};
        double value = std::strtod(next.buffer, &end);
        if (end == (next.buffer + next.len)) {
            visitor.visit_f64(value);
        } else {
            throw SPrintfDeserializationException("element is not a float/double or is too long");
        }
    }
}
void SPrintfDeserializer::deserialize_char(de::Visitor & visitor) {
    DelimitedString next = next_delimited_string();
    if (next.len == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else if (next.len != 1) {
        throw SPrintfDeserializationException("element is not a character");
    } else {
        visitor.visit_char(*next.buffer);
    }
}
void SPrintfDeserializer::deserialize_string(de::Visitor & visitor) {
    DelimitedString next = next_delimited_string();
    if (next.len == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        visitor.visit_c_str(next.buffer, next.len);
    }
}
void SPrintfDeserializer::deserialize_seq(de::Visitor & visitor) {
    SPrintfSeqAccess seq(*this);
    visitor.visit_seq(seq);
}
void SPrintfDeserializer::deserialize_map(de::Visitor & visitor) {
    SPrintfMapAccess map(*this);
    visitor.visit_map(map);
}
void SPrintfDeserializer::deserialize_struct(
    const char* name,
    const FieldNames & field_names,
    de::Visitor & visitor)
{
    // TODO
    deserialize_map(visitor);
}

SPrintfDeserializer::SPrintfSeqAccess::SPrintfSeqAccess(SPrintfDeserializer & parent)
    : parent(parent), index(0), count(0)
{
    // Get the number of elements in the sequence
    de::deserialize(parent, count);
}
bool SPrintfDeserializer::SPrintfSeqAccess::has_next() {
    return index < count;
}
void SPrintfDeserializer::SPrintfSeqAccess::next_element(de::Deserialize & element) {
    if (has_next()) {
        element.deserialize(parent);
        ++index;
    } else {
        throw SPrintfDeserializationException("end of sequence reached");
    }
}

SPrintfDeserializer::SPrintfMapAccess::SPrintfMapAccess(SPrintfDeserializer & parent)
    : parent(parent), index(0), count(0)
{
    // Get the number of entries in the map
    de::deserialize(parent, count);
}
bool SPrintfDeserializer::SPrintfMapAccess::has_next() {
    return index < count;
}
void SPrintfDeserializer::SPrintfMapAccess::next_key(de::Deserialize & key) {
    if (has_next()) {
        key.deserialize(parent);
    } else {
        throw SPrintfDeserializationException("end of map reached");
    }
}
void SPrintfDeserializer::SPrintfMapAccess::next_value(de::Deserialize & value) {
    if (has_next()) {
        value.deserialize(parent);
        ++index;
    } else {
        throw SPrintfDeserializationException("end of map reached");
    }
}
void SPrintfDeserializer::SPrintfMapAccess::next_entry(de::Deserialize & key, de::Deserialize & value) {
    next_key(key);
    next_value(value);
}

SPrintfDeserializer::DelimitedString SPrintfDeserializer::next_delimited_string() {
    const char* iter = buffer;
    while (iter != buffer_end && *iter != '\0') { ++iter; }
    
    DelimitedString result{ buffer, static_cast<std::size_t>(iter - buffer) };
    buffer = (iter == buffer_end) ? buffer_end : iter + 1;
    return result;
}

}  // namespace kingw
