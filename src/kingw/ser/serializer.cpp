#include "kingw/ser/serializer.hpp"


namespace kingw {
namespace ser {

SerializationException::SerializationException(const std::string & message)
    : std::runtime_error(message) { }

Serializer::SerializeSeq Serializer::serialize_seq() {
    return SerializeSeq{ *this };
}
Serializer::SerializeSeq::SerializeSeq(Serializer & serializer) : serializer(serializer) {
    serializer.seq_begin();
}
Serializer::SerializeSeq::~SerializeSeq() {
    end();
}
void Serializer::SerializeSeq::serialize_element(const Serialize & accessor) {
    if (!closed) {
        serializer.seq_serialize_element(accessor);
    }
}
void Serializer::SerializeSeq::end() {
    if (!closed) {
        serializer.seq_end();
        closed = true;
    }
}

Serializer::SerializeMap Serializer::serialize_map() {
    return SerializeMap{ *this };
}
Serializer::SerializeMap::SerializeMap(Serializer & serializer) : serializer(serializer) {
    serializer.map_begin();
}
Serializer::SerializeMap::~SerializeMap() {
    end();
}
void Serializer::SerializeMap::serialize_key(const Serialize & key) {
    if (!closed) {
        serializer.map_serialize_key(key);
    }
}
void Serializer::SerializeMap::serialize_value(const Serialize & value) {
    if (!closed) {
        serializer.map_serialize_value(value);
    }
}
void Serializer::SerializeMap::serialize_entry(const Serialize & key, const Serialize & value) {
    if (!closed) {
        serializer.map_serialize_entry(key, value);
    }
}
void Serializer::SerializeMap::end() {
    if (!closed) {
        serializer.map_end();
        closed = true;
    }
}

Serializer::SerializeStruct Serializer::serialize_struct() {
    return SerializeStruct{ *this };
}
Serializer::SerializeStruct::SerializeStruct(Serializer & serializer) : serializer(serializer) {
    serializer.struct_begin();
}
Serializer::SerializeStruct::~SerializeStruct() {
    end();
}
void Serializer::SerializeStruct::serialize_field(const char * name, const Serialize & accessor) {
    if (!closed) {
        serializer.struct_serialize_field(name, accessor);
    }
}
void Serializer::SerializeStruct::skip_field(const char * name) {
    if (!closed) {
        serializer.struct_skip_field(name);
    }
}
void Serializer::SerializeStruct::end() {
    if (!closed) {
        serializer.struct_end();
        closed = true;
    }
}

template <>
void serialize<Serialize>(Serializer & serializer, const Serialize & accessor) {
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

}
}
