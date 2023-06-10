#include "kingw/ostream_serializer.hpp"

#include <cstring>


namespace kingw {

OStreamSerializer::OStreamSerializer(std::ostream & stream, bool human_readable)
    : stream(stream), human_readable(human_readable) { }

bool OStreamSerializer::is_human_readable() const {
    return human_readable;
}

void OStreamSerializer::serialize_bool(bool value) {
    stream << value;
}
void OStreamSerializer::serialize_i8(std::int8_t value) {
    stream << value;
}
void OStreamSerializer::serialize_i16(std::int16_t value) {
    stream << value;
}
void OStreamSerializer::serialize_i32(std::int32_t value) {
    stream << value;
}
void OStreamSerializer::serialize_i64(std::int64_t value) {
    stream << value;
}
void OStreamSerializer::serialize_u8(std::uint8_t value) {
    stream << value;
}
void OStreamSerializer::serialize_u16(std::uint16_t value) {
    stream << value;
}
void OStreamSerializer::serialize_u32(std::uint32_t value) {
    stream << value;
}
void OStreamSerializer::serialize_u64(std::uint64_t value) {
    stream << value;
}
void OStreamSerializer::serialize_f32(float value) {
    stream << value;
}
void OStreamSerializer::serialize_f64(double value) {
    stream << value;
}
void OStreamSerializer::serialize_char(char value) {
    stream << value;
}
void OStreamSerializer::serialize_string(const char* begin, const char* end) {
    stream.write(begin, end - begin);
}


///
/// Sequences 
///

void OStreamSerializer::seq_begin(std::size_t len) {
    // No-op
}

void OStreamSerializer::seq_serialize_element(const ser::Serialize & element) {
    element.serialize(*this);
}

void OStreamSerializer::seq_end() {
    // No-op
}


///
/// Maps 
///

void OStreamSerializer::map_begin(std::size_t len) {
    // No-op
}

void OStreamSerializer::map_serialize_key(const ser::Serialize & key) {
    key.serialize(*this);
}

void OStreamSerializer::map_serialize_value(const ser::Serialize & value) {
    value.serialize(*this);
}

void OStreamSerializer::map_serialize_entry(const ser::Serialize & key, const ser::Serialize & value) {
    map_serialize_key(key);
    map_serialize_value(value);
}

void OStreamSerializer::map_end() {
    // No-op
}


///
/// Structs 
///

void OStreamSerializer::struct_begin(const char* name, std::size_t len) {
    // No-op
}

void OStreamSerializer::struct_serialize_field(const char * name, const ser::Serialize & field) {
    std::size_t len = std::strlen(name);
    serialize_string(name, name + len);
    field.serialize(*this);
}

void OStreamSerializer::struct_skip_field(const char * name) {
    // No-op
}

void OStreamSerializer::struct_end() {
    // No-op
}

}  // namespace kingw
