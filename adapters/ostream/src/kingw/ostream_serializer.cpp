#include "kingw/ostream_serializer.hpp"


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
void OStreamSerializer::serialize_c_str(const char * value) {
    stream << value;
}
void OStreamSerializer::serialize_string(const std::string & value) {
    stream << value;
}


///
/// Sequences 
///

void OStreamSerializer::serialize_seq_begin() {
    // No-op
}

void OStreamSerializer::serialize_seq_element(const ser::Serialize & accessor) {
    accessor.serialize(*this);
}

void OStreamSerializer::serialize_seq_end() {
    // No-op
}


///
/// Maps 
///

void OStreamSerializer::serialize_map_begin() {
    // No-op
}

void OStreamSerializer::serialize_map_key(const ser::Serialize & accessor) {
    accessor.serialize(*this);
}

void OStreamSerializer::serialize_map_value(const ser::Serialize & accessor) {
    accessor.serialize(*this);
}

void OStreamSerializer::serialize_map_end() {
    // No-op
}


///
/// Structs 
///

void OStreamSerializer::serialize_struct_begin() {
    // No-op
}

void OStreamSerializer::serialize_struct_field(const ser::Serialize & accessor, const char * name) {
    serialize_c_str(name);
    accessor.serialize(*this);
}

void OStreamSerializer::serialize_struct_end() {
    // No-op
}

}  // namespace kingw
