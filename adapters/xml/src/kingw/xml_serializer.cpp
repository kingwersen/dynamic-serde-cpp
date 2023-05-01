#include "kingw/xml_serializer.hpp"


namespace kingw {

XmlSerializer::SerializationException::SerializationException(const std::string & str)
    : std::runtime_error(str) { }

XmlSerializer::XmlSerializer(std::ostream & stream)
    : stream(stream) { }

bool XmlSerializer::is_human_readable() const {
    return true;
}

void XmlSerializer::serialize_bool(bool value) {
    stream << value;
}
void XmlSerializer::serialize_i8(std::int8_t value) {
    stream << value;
}
void XmlSerializer::serialize_i16(std::int16_t value) {
    stream << value;
}
void XmlSerializer::serialize_i32(std::int32_t value) {
    stream << value;
}
void XmlSerializer::serialize_i64(std::int64_t value) {
    stream << value;
}
void XmlSerializer::serialize_u8(std::uint8_t value) {
    stream << value;
}
void XmlSerializer::serialize_u16(std::uint16_t value) {
    stream << value;
}
void XmlSerializer::serialize_u32(std::uint32_t value) {
    stream << value;
}
void XmlSerializer::serialize_u64(std::uint64_t value) {
    stream << value;
}
void XmlSerializer::serialize_f32(float value) {
    stream << value;
}
void XmlSerializer::serialize_f64(double value) {
    stream << value;
}
void XmlSerializer::serialize_char(char value) {
    stream << value;
}
void XmlSerializer::serialize_c_str(const char * value) {
    stream << value;
}
void XmlSerializer::serialize_string(const std::string & value) {
    stream << value;
}


///
/// Sequences 
///

void XmlSerializer::serialize_seq_begin() {
}
void XmlSerializer::serialize_seq_element(const ser::Serialize & accessor) {
    stream << "<element>";
    accessor.serialize(*this);
    stream << "</element>";
}
void XmlSerializer::serialize_seq_end() {
}


///
/// Maps 
///

void XmlSerializer::serialize_map_begin() {
}
void XmlSerializer::serialize_map_key(const ser::Serialize & accessor) {
    if (!accessor.traits().is_string) {
        throw SerializationException("XmlSerializer map key is not a string");
    }
    auto tag = XmlSerializer::to_string(accessor);
    stream << "<" << tag << ">";
    open_tags.push(std::move(tag));
}
void XmlSerializer::serialize_map_value(const ser::Serialize & accessor) {
    accessor.serialize(*this);
}
void XmlSerializer::serialize_map_end() {
    stream << "</" << open_tags.top() << ">";
    open_tags.pop();
}


///
/// Structs 
///

void XmlSerializer::serialize_struct_begin() {
}
void XmlSerializer::serialize_struct_field(const ser::Serialize & accessor, const char * name) {
    stream << "<";
    serialize_c_str(name);
    stream << ">";
    accessor.serialize(*this);
    stream << "</";
    serialize_c_str(name);
    stream << ">";
}
void XmlSerializer::serialize_struct_end() {
}

}  // namespace kingw
