#include "kingw/serde/serialize.hpp"

#include <cstdint>


namespace kingw {
namespace ser {

Serializer::Seq Serializer::serialize_seq() {
    return Seq{ *this };
}
Serializer::Seq::Seq(Serializer & serializer) : serializer(serializer) {
    serializer.serialize_seq_begin();
}
Serializer::Seq::~Seq() {
    serializer.serialize_seq_end();
}
void Serializer::Seq::serialize_element(const Dynamic & accessor) {
    serializer.serialize_seq_element(accessor);
}

Serializer::Map Serializer::serialize_map() {
    return Map{ *this };
}
Serializer::Map::Map(Serializer & serializer) : serializer(serializer) {
    serializer.serialize_map_begin();
}
Serializer::Map::~Map() {
    serializer.serialize_map_end();
}
void Serializer::Map::serialize_key(const Dynamic & accessor) {
    serializer.serialize_map_key(accessor);
}
void Serializer::Map::serialize_value(const Dynamic & accessor) {
    serializer.serialize_map_value(accessor);
}

Serializer::Struct Serializer::serialize_struct() {
    return Struct{ *this };
}
Serializer::Struct::Struct(Serializer & serializer) : serializer(serializer) {
    serializer.serialize_struct_begin();
}
Serializer::Struct::~Struct() {
    serializer.serialize_struct_end();
}
void Serializer::Struct::serialize_field(const Dynamic & accessor, const char * name) {
    serializer.serialize_struct_field(accessor, name);
}

template <>
void serialize<Dynamic>(Serializer & serializer, const Dynamic & accessor) {
    accessor.serialize(serializer);
}
template <>
void serialize<bool>(Serializer & serializer, const bool & data) {
    serializer.serialize_bool(data);
}
template <>
void serialize<std::int8_t>(Serializer & serializer, const std::int8_t & data) {
    serializer.serialize_i8(data);
}
template <>
void serialize<std::int16_t>(Serializer & serializer, const std::int16_t & data) {
    serializer.serialize_i16(data);
}
template <>
void serialize<std::int32_t>(Serializer & serializer, const std::int32_t & data) {
    serializer.serialize_i32(data);
}
template <>
void serialize<std::int64_t>(Serializer & serializer, const std::int64_t & data) {
    serializer.serialize_i64(data);
}
template <>
void serialize<std::uint8_t>(Serializer & serializer, const std::uint8_t & data) {
    serializer.serialize_u8(data);
}
template <>
void serialize<std::uint16_t>(Serializer & serializer, const std::uint16_t & data) {
    serializer.serialize_u16(data);
}
template <>
void serialize<std::uint32_t>(Serializer & serializer, const std::uint32_t & data) {
    serializer.serialize_u32(data);
}
template <>
void serialize<std::uint64_t>(Serializer & serializer, const std::uint64_t & data) {
    serializer.serialize_u64(data);
}
template <>
void serialize<float>(Serializer & serializer, const float & data) {
    serializer.serialize_f32(data);
}
template <>
void serialize<double>(Serializer & serializer, const double & data) {
    serializer.serialize_f64(data);
}
template <>
void serialize<char>(Serializer & serializer, const char & data) {
    serializer.serialize_char(data);
}
template <>
void serialize<std::string>(Serializer & serializer, const std::string & data) {
    serializer.serialize_string(data);
}

}  // namespace ser
}  // namespace kingw
