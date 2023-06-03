#include "kingw/de/deserializer.hpp"

#include <limits>
#include <cstring>

#include "kingw/de/integral_visitors.hpp"


namespace kingw {
namespace de {

DeserializationException::DeserializationException(const char* message)
    : std::runtime_error(message) { }

Visitor::NotImplementedException::NotImplementedException(const char* message)
    : DeserializationException(message) { }

// Default implementations for unused visitor functions.
// If they are called when not implemented, throw a runtime exception.
void Visitor::visit_bool(bool value) { throw NotImplementedException("visitor unexpected type bool"); }
void Visitor::visit_i8(std::int8_t value) { throw NotImplementedException("visitor unexpected type i8"); }
void Visitor::visit_i16(std::int16_t value) { throw NotImplementedException("visitor unexpected type i16"); }
void Visitor::visit_i32(std::int32_t value) { throw NotImplementedException("visitor unexpected type i32"); }
void Visitor::visit_i64(std::int64_t value) { throw NotImplementedException("visitor unexpected type i64"); }
void Visitor::visit_u8(std::uint8_t value) { throw NotImplementedException("visitor unexpected type u8"); }
void Visitor::visit_u16(std::uint16_t value) { throw NotImplementedException("visitor unexpected type u16"); }
void Visitor::visit_u32(std::uint32_t value) { throw NotImplementedException("visitor unexpected type u32"); }
void Visitor::visit_u64(std::uint64_t value) { throw NotImplementedException("visitor unexpected type u64"); }
void Visitor::visit_f32(float value) { throw NotImplementedException("visitor unexpected type f32"); }
void Visitor::visit_f64(double value) { throw NotImplementedException("visitor unexpected type f64"); }
void Visitor::visit_char(char value) { throw NotImplementedException("visitor unexpected type char"); }
void Visitor::visit_string(const std::string & value) { throw NotImplementedException("visitor unexpected type string"); }
void Visitor::visit_c_str(const char* buffer, std::size_t len) { throw NotImplementedException("visitor unexpected type c string"); }
void Visitor::visit_seq(Deserializer::SeqAccess & value) { throw NotImplementedException("visitor unexpected type seq"); }
void Visitor::visit_map(Deserializer::MapAccess & value) { throw NotImplementedException("visitor unexpected type map"); }


BoolVisitor::BoolVisitor(bool & output) 
    : output(output) { }
const char* BoolVisitor::expecting() const {
    return "a boolean";
}
void BoolVisitor::visit_bool(bool value) {
    output = value;
}

/// The logic for all number-to-number conversions is similar.
/// If the value of the OTHER number is within the bounds of SELF,
/// then it's OK to copy OTHER into SELF.
/// Otherwise, throw a runtime exception.
///
/// These defines are #undef'd later.
#define KINGW_NUM_AS_SELF(CLASS, TYPE, FN)                      \
    void CLASS::FN(TYPE value) {                                \
        output = value;                                         \
    }
#define KINGW_TRY_NUM_INTO_SELF(CLASS, SELF, OTHER, FN)         \
    void CLASS::FN(OTHER value) {                               \
        if (   value >= std::numeric_limits<SELF>::min()        \
            && value <= std::numeric_limits<SELF>::max()) {     \
            output = static_cast<SELF>(value);                  \
        } else {                                                \
            throw DeserializationException("number outside range"); \
        }                                                       \
    }

I8Visitor::I8Visitor(std::int8_t & output) 
    : output(output) { }
const char* I8Visitor::expecting() const {
    return "an 8-bit integer";
}
KINGW_NUM_AS_SELF(I8Visitor, std::int8_t, visit_i8);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::int16_t, visit_i16);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::int64_t, visit_i64);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::uint8_t, visit_u8);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::uint16_t, visit_u16);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(I8Visitor, std::int8_t, std::uint64_t, visit_u64);

I16Visitor::I16Visitor(std::int16_t & output) 
    : output(output) { }
const char* I16Visitor::expecting() const {
    return "a 16-bit integer";
}
KINGW_NUM_AS_SELF(I16Visitor, std::int8_t, visit_i8);
KINGW_NUM_AS_SELF(I16Visitor, std::int16_t, visit_i16);
KINGW_TRY_NUM_INTO_SELF(I16Visitor, std::int16_t, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(I16Visitor, std::int16_t, std::int64_t, visit_i64);
KINGW_TRY_NUM_INTO_SELF(I16Visitor, std::int16_t, std::uint8_t, visit_u8);
KINGW_TRY_NUM_INTO_SELF(I16Visitor, std::int16_t, std::uint16_t, visit_u16);
KINGW_TRY_NUM_INTO_SELF(I16Visitor, std::int16_t, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(I16Visitor, std::int16_t, std::uint64_t, visit_u64);

I32Visitor::I32Visitor(std::int32_t & output)
    : output(output) { }
const char* I32Visitor::expecting() const {
    return "a 32-bit integer";
}
KINGW_NUM_AS_SELF(I32Visitor, std::int8_t, visit_i8);
KINGW_NUM_AS_SELF(I32Visitor, std::int16_t, visit_i16);
KINGW_NUM_AS_SELF(I32Visitor, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(I32Visitor, std::int32_t, std::int64_t, visit_i64);
KINGW_TRY_NUM_INTO_SELF(I32Visitor, std::int32_t, std::uint8_t, visit_u8);
KINGW_TRY_NUM_INTO_SELF(I32Visitor, std::int32_t, std::uint16_t, visit_u16);
KINGW_TRY_NUM_INTO_SELF(I32Visitor, std::int32_t, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(I32Visitor, std::int32_t, std::uint64_t, visit_u64);

I64Visitor::I64Visitor(std::int64_t & output)
    : output(output) { }
const char* I64Visitor::expecting() const {
    return "a 64-bit integer";
}
KINGW_NUM_AS_SELF(I64Visitor, std::int8_t, visit_i8);
KINGW_NUM_AS_SELF(I64Visitor, std::int16_t, visit_i16);
KINGW_NUM_AS_SELF(I64Visitor, std::int32_t, visit_i32);
KINGW_NUM_AS_SELF(I64Visitor, std::int64_t, visit_i64);
KINGW_TRY_NUM_INTO_SELF(I64Visitor, std::int64_t, std::uint8_t, visit_u8);
KINGW_TRY_NUM_INTO_SELF(I64Visitor, std::int64_t, std::uint16_t, visit_u16);
KINGW_TRY_NUM_INTO_SELF(I64Visitor, std::int64_t, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(I64Visitor, std::int64_t, std::uint64_t, visit_u64);

U8Visitor::U8Visitor(std::uint8_t & output)
    : output(output) { }
const char* U8Visitor::expecting() const {
    return "an 8-bit unsigned integer";
}
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::int8_t, visit_i8);
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::int16_t, visit_i16);
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::int64_t, visit_i64);
KINGW_NUM_AS_SELF(U8Visitor, std::uint8_t, visit_u8);
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::uint16_t, visit_u16);
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(U8Visitor, std::uint8_t, std::uint64_t, visit_u64);

U16Visitor::U16Visitor(std::uint16_t & output) 
    : output(output) { }
const char* U16Visitor::expecting() const {
    return "a 16-bit unsigned integer";
}
KINGW_TRY_NUM_INTO_SELF(U16Visitor, std::uint16_t, std::int8_t, visit_i8);
KINGW_TRY_NUM_INTO_SELF(U16Visitor, std::uint16_t, std::int16_t, visit_i16);
KINGW_TRY_NUM_INTO_SELF(U16Visitor, std::uint16_t, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(U16Visitor, std::uint16_t, std::int64_t, visit_i64);
KINGW_NUM_AS_SELF(U16Visitor, std::uint8_t, visit_u8);
KINGW_NUM_AS_SELF(U16Visitor, std::uint16_t, visit_u16);
KINGW_TRY_NUM_INTO_SELF(U16Visitor, std::uint16_t, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(U16Visitor, std::uint16_t, std::uint64_t, visit_u64);

U32Visitor::U32Visitor(std::uint32_t & output)
    : output(output) { }
const char* U32Visitor::expecting() const {
    return "a 32-bit unsigned integer";
}
KINGW_TRY_NUM_INTO_SELF(U32Visitor, std::uint32_t, std::int8_t, visit_i8);
KINGW_TRY_NUM_INTO_SELF(U32Visitor, std::uint32_t, std::int16_t, visit_i16);
KINGW_TRY_NUM_INTO_SELF(U32Visitor, std::uint32_t, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(U32Visitor, std::uint32_t, std::int64_t, visit_i64);
KINGW_NUM_AS_SELF(U32Visitor, std::uint8_t, visit_u8);
KINGW_NUM_AS_SELF(U32Visitor, std::uint16_t, visit_u16);
KINGW_NUM_AS_SELF(U32Visitor, std::uint32_t, visit_u32);
KINGW_TRY_NUM_INTO_SELF(U32Visitor, std::uint32_t, std::uint64_t, visit_u64);

U64Visitor::U64Visitor(std::uint64_t & output)
    : output(output) { }
const char* U64Visitor::expecting() const {
    return "a 64-bit unsigned integer";
}
KINGW_TRY_NUM_INTO_SELF(U64Visitor, std::uint64_t, std::int8_t, visit_i8);
KINGW_TRY_NUM_INTO_SELF(U64Visitor, std::uint64_t, std::int16_t, visit_i16);
KINGW_TRY_NUM_INTO_SELF(U64Visitor, std::uint64_t, std::int32_t, visit_i32);
KINGW_TRY_NUM_INTO_SELF(U64Visitor, std::uint64_t, std::int64_t, visit_i64);
KINGW_NUM_AS_SELF(U64Visitor, std::uint8_t, visit_u8);
KINGW_NUM_AS_SELF(U64Visitor, std::uint16_t, visit_u16);
KINGW_NUM_AS_SELF(U64Visitor, std::uint32_t, visit_u32);
KINGW_NUM_AS_SELF(U64Visitor, std::uint64_t, visit_u64);

F32Visitor::F32Visitor(float & output)
    : output(output) { }
const char* F32Visitor::expecting() const {
    return "a 32-bit float";
}
KINGW_NUM_AS_SELF(F32Visitor, float, visit_f32);
KINGW_NUM_AS_SELF(F32Visitor, double, visit_f64);
KINGW_NUM_AS_SELF(F32Visitor, std::int8_t, visit_i8);
KINGW_NUM_AS_SELF(F32Visitor, std::int16_t, visit_i16);
KINGW_NUM_AS_SELF(F32Visitor, std::int32_t, visit_i32);
KINGW_NUM_AS_SELF(F32Visitor, std::int64_t, visit_i64);
KINGW_NUM_AS_SELF(F32Visitor, std::uint8_t, visit_u8);
KINGW_NUM_AS_SELF(F32Visitor, std::uint16_t, visit_u16);
KINGW_NUM_AS_SELF(F32Visitor, std::uint32_t, visit_u32);
KINGW_NUM_AS_SELF(F32Visitor, std::uint64_t, visit_u64);

F64Visitor::F64Visitor(double & output)
    : output(output) { }
const char* F64Visitor::expecting() const {
    return "a 64-bit float";
}
KINGW_NUM_AS_SELF(F64Visitor, float, visit_f32);
KINGW_NUM_AS_SELF(F64Visitor, double, visit_f64);
KINGW_NUM_AS_SELF(F64Visitor, std::int8_t, visit_i8);
KINGW_NUM_AS_SELF(F64Visitor, std::int16_t, visit_i16);
KINGW_NUM_AS_SELF(F64Visitor, std::int32_t, visit_i32);
KINGW_NUM_AS_SELF(F64Visitor, std::int64_t, visit_i64);
KINGW_NUM_AS_SELF(F64Visitor, std::uint8_t, visit_u8);
KINGW_NUM_AS_SELF(F64Visitor, std::uint16_t, visit_u16);
KINGW_NUM_AS_SELF(F64Visitor, std::uint32_t, visit_u32);
KINGW_NUM_AS_SELF(F64Visitor, std::uint64_t, visit_u64);

#undef KINGW_TRY_NUM_INTO_SELF
#undef KINGW_NUM_AS_SELF

CharVisitor::CharVisitor(char & output)
    : output(output) { }
const char* CharVisitor::expecting() const {
    return "a character";
}
void CharVisitor::visit_char(char value) {
    output = value;
}
void CharVisitor::visit_c_str(const char* value, std::size_t len) {
    if (len == -1) { len = std::strlen(value); }
    if (len == 1) {
        output = value[0];
    } else {
        throw DeserializationException("string does not contain exactly one character");
    }
}
void CharVisitor::visit_string(const std::string & value) {
    if (value.size() == 1) {
        output = value[0];
    } else {
        throw DeserializationException("string does not contain exactly one character");
    }
}

StringVisitor::StringVisitor(std::string & output)
    : output(output) { }
const char* StringVisitor::expecting() const {
    return "a string";
}
void StringVisitor::visit_c_str(const char* value, std::size_t len) {
    visit_string(value);
}
void StringVisitor::visit_string(const std::string & value) {
    output = value;
}

CStringVisitor::CStringVisitor(char* output, std::size_t output_len)
    : output(output), output_len(output_len) { }
const char* CStringVisitor::expecting() const {
    return "a string";
}
void CStringVisitor::visit_c_str(const char* value, std::size_t len) {
    if (len == -1) { len = std::strlen(value); }
    if (len <= output_len) {
        std::snprintf(output, output_len, "%s", value);
    } else {
        throw DeserializationException("deserialized string doesn't fit in fixed-size buffer");
    }
}
void CStringVisitor::visit_string(const std::string & value) {
    visit_c_str(value.c_str(), value.size());
}


template <>
void deserialize<Deserialize>(Deserializer & deserializer, Deserialize & accessor) {
    accessor.deserialize(deserializer);
}
template <>
void deserialize<bool>(Deserializer & deserializer, bool & data) {
    BoolVisitor visitor(data);
    deserializer.deserialize_bool(visitor);
}
template <>
void deserialize<std::int8_t>(Deserializer & deserializer, std::int8_t & data) {
    I8Visitor visitor(data);
    deserializer.deserialize_i8(visitor);
}
template <>
void deserialize<std::int16_t>(Deserializer & deserializer, std::int16_t & data) {
    I16Visitor visitor(data);
    deserializer.deserialize_i16(visitor);
}
template <>
void deserialize<std::int32_t>(Deserializer & deserializer, std::int32_t & data) {
    I32Visitor visitor(data);
    deserializer.deserialize_i32(visitor);
}
template <>
void deserialize<std::int64_t>(Deserializer & deserializer, std::int64_t & data) {
    I64Visitor visitor(data);
    deserializer.deserialize_i64(visitor);
}
template <>
void deserialize<std::uint8_t>(Deserializer & deserializer, std::uint8_t & data) {
    U8Visitor visitor(data);
    deserializer.deserialize_u8(visitor);
}
template <>
void deserialize<std::uint16_t>(Deserializer & deserializer, std::uint16_t & data) {
    U16Visitor visitor(data);
    deserializer.deserialize_u16(visitor);
}
template <>
void deserialize<std::uint32_t>(Deserializer & deserializer, std::uint32_t & data) {
    U32Visitor visitor(data);
    deserializer.deserialize_u32(visitor);
}
template <>
void deserialize<std::uint64_t>(Deserializer & deserializer, std::uint64_t & data) {
    U64Visitor visitor(data);
    deserializer.deserialize_u64(visitor);
}
template <>
void deserialize<float>(Deserializer & deserializer, float & data) {
    F32Visitor visitor(data);
    deserializer.deserialize_f32(visitor);
}
template <>
void deserialize<double>(Deserializer & deserializer, double & data) {
    F64Visitor visitor(data);
    deserializer.deserialize_f64(visitor);
}
template <>
void deserialize<char>(Deserializer & deserializer, char & data) {
    CharVisitor visitor(data);
    deserializer.deserialize_char(visitor);
}
template <>
void deserialize<std::string>(Deserializer & deserializer, std::string & data) {
    StringVisitor visitor(data);
    deserializer.deserialize_string(visitor);
}

}  // namespace de
}  // namespace kingw
