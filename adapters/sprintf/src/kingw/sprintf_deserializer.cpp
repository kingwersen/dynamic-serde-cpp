#include "kingw/sprintf_deserializer.hpp"

#include "kingw/de/accessor.hpp"


namespace kingw {
namespace serde_sprintf {

SPrintfDeserializer::SPrintfDeserializationException::SPrintfDeserializationException(serde::string_view message)
    : de::DeserializationException(message) { }

SPrintfDeserializer::SPrintfDeserializer(serde::string_view input, bool human_readable)
    : buffer(input), last_end_(buffer.begin()), human_readable(human_readable) { }

bool SPrintfDeserializer::is_human_readable() const {
    return human_readable;
}

const char* SPrintfDeserializer::last_end() const {
    return last_end_;
}

// Basic Types
void SPrintfDeserializer::deserialize_any(de::Visitor & visitor) {
    throw SPrintfDeserializationException("deserialize_any() not implemented");
}
void SPrintfDeserializer::deserialize_bool(de::Visitor & visitor) {
    serde::string_view next = next_delimited_string();
    if (next.size() == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else if (next.size() != 1 || next[0] != '0' || next[0] != '1') {
        throw SPrintfDeserializationException("element is not a boolean");
    } else {
        visitor.visit_bool(next[0] == '1');
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
    serde::string_view next = next_delimited_string();
    if (next.size() == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        char* end{};
        std::int64_t value = std::strtol(next.begin(), &end, 10);
        if (end == next.end()) {
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
    serde::string_view next = next_delimited_string();
    if (next.size() == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        char* end{};
        std::uint64_t value = std::strtoul(next.begin(), &end, 10);
        if (end == next.end()) {
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
    serde::string_view next = next_delimited_string();
    if (next.size() == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        char* end{};
        double value = std::strtod(next.begin(), &end);
        if (end == next.end()) {
            visitor.visit_f64(value);
        } else {
            throw SPrintfDeserializationException("element is not a float/double or is too long");
        }
    }
}
void SPrintfDeserializer::deserialize_char(de::Visitor & visitor) {
    serde::string_view next = next_delimited_string();
    if (next.size() == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else if (next.size() != 1) {
        throw SPrintfDeserializationException("element is not a character");
    } else {
        visitor.visit_char(next[0]);
    }
}
void SPrintfDeserializer::deserialize_string(de::Visitor & visitor) {
    serde::string_view next = next_delimited_string();
    if (next.size() == 0) {
        throw SPrintfDeserializationException("buffer is empty");
    } else {
        visitor.visit_string(next);
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
    serde::string_view name,
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

serde::string_view SPrintfDeserializer::next_delimited_string() {
    // Find the next EOF, or the end of the input.
    const char* iter = buffer.begin();
    while (iter != buffer.end() && *iter != '\0') { ++iter; }
    
    // Prepare to return a string_view to this data.
    serde::string_view result(buffer.begin(), iter - buffer.begin());
    last_end_ = result.end();

    // Advance input to just beyond the EOF, or to the end of the input.
    if (iter == buffer.end()) {
        buffer = serde::string_view(buffer.end(), 0);
    } else {
        ++iter;  // Skip '\0'
        buffer = serde::string_view(iter, buffer.end() - iter);
    }
    return result;
}

}  // namespace serde_sprintf
}  // namespace kingw
