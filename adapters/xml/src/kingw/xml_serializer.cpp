#include "kingw/xml_serializer.hpp"

#include <cstring>


namespace kingw {

XmlSerializer::XmlSerializationException::XmlSerializationException(const char* message)
    : std::runtime_error(message) { }

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
void XmlSerializer::serialize_string(const char* begin, const char* end) {
    stream.write(begin, end - begin);
}


///
/// Sequences 
///

void XmlSerializer::seq_begin(std::size_t len) {
}
void XmlSerializer::seq_serialize_element(const ser::Serialize & element) {
    stream << "<element>";
    element.serialize(*this);
    stream << "</element>";
}
void XmlSerializer::seq_end() {
}


///
/// Maps 
///

void XmlSerializer::map_begin(std::size_t len) {
}
void XmlSerializer::map_serialize_key(const ser::Serialize & key) {
    if (!key.traits().is_string) {
        throw XmlSerializationException("XmlSerializer map key is not a string");
    }
    auto tag = XmlSerializer::to_string(key);
    stream << "<" << tag << ">";
    open_tags.push(std::move(tag));
}
void XmlSerializer::map_serialize_value(const ser::Serialize & value) {
    value.serialize(*this);
}
void XmlSerializer::map_serialize_entry(const ser::Serialize & key, const ser::Serialize & value) {
    map_serialize_key(key);
    map_serialize_value(value);
}
void XmlSerializer::map_end() {
    stream << "</" << open_tags.top() << ">";
    open_tags.pop();
}


///
/// Structs 
///

void XmlSerializer::struct_begin(const char* name, std::size_t len) {
}
void XmlSerializer::struct_serialize_field(const char * name, const ser::Serialize & field) {
    stream << "<";
    serialize_string(name, name + std::strlen(name));
    stream << ">";
    field.serialize(*this);
    stream << "</";
    serialize_string(name, name + std::strlen(name));
    stream << ">";
}
void XmlSerializer::struct_skip_field(const char * name) {
    // No-op
}
void XmlSerializer::struct_end() {
}

}  // namespace kingw
