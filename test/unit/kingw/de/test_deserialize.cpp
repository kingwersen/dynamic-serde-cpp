#include <limits>

#include <gmock/gmock.h>

#include "kingw/mock/de/mock_deserializer.hpp"

using namespace kingw::de;


namespace {

/// All functions of Visitor should throw NotImplementedException if not overridden.
///
TEST(KingwSerde, VisitorNotImplemented) {
    struct VisitorImpl : public Visitor {
        const char* expecting() const override { return ""; }
    };
    VisitorImpl visitor;

    MockSeqAccess mock_seq_access;
    MockMapAccess mock_map_access;

    EXPECT_THROW(visitor.visit_bool(false), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f32(0.0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f64(0.0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_char('\0'), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_string(""), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_seq(mock_seq_access), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_map(mock_map_access), Visitor::NotImplementedException);
}

/// deserialize<bool>(deserializer, value) will invoke deserializer.deserialize_bool(value)
///
TEST(KingwSerde, DeserializeBool) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_bool)
        .Times(1);

    bool data = false;
    deserialize<bool>(mock_deserializer, data);
}

/// deserialize<std::int8_t>(deserializer, value) will invoke deserializer.deserialize_i8(value)
///
TEST(KingwSerde, DeserializeI8) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_i8)
        .Times(1);

    std::int8_t data = 0;
    deserialize<std::int8_t>(mock_deserializer, data);
}

/// deserialize<std::int16_t>(deserializer, value) will invoke deserializer.deserialize_i16(value)
///
TEST(KingwSerde, DeserializeI16) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_i16)
        .Times(1);

    std::int16_t data = 0;
    deserialize<std::int16_t>(mock_deserializer, data);
}

/// deserialize<std::int32_t>(deserializer, value) will invoke deserializer.deserialize_i32(value)
///
TEST(KingwSerde, DeserializeI32) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_i32)
        .Times(1);

    std::int32_t data = 0;
    deserialize<std::int32_t>(mock_deserializer, data);
}

/// deserialize<std::int64_t>(deserializer, value) will invoke deserializer.deserialize_i64(value)
///
TEST(KingwSerde, DeserializeI64) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_i64)
        .Times(1);

    std::int64_t data = 0;
    deserialize<std::int64_t>(mock_deserializer, data);
}

/// deserialize<std::uint8_t>(deserializer, value) will invoke deserializer.deserialize_u8(value)
///
TEST(KingwSerde, DeserializeU8) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_u8)
        .Times(1);

    std::uint8_t data = 0;
    deserialize<std::uint8_t>(mock_deserializer, data);
}

/// deserialize<std::uint16_t>(deserializer, value) will invoke deserializer.deserialize_u16(value)
///
TEST(KingwSerde, DeserializeU16) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_u16)
        .Times(1);

    std::uint16_t data = 0;
    deserialize<std::uint16_t>(mock_deserializer, data);
}

/// deserialize<std::uint32_t>(deserializer, value) will invoke deserializer.deserialize_u32(value)
///
TEST(KingwSerde, DeserializeU32) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_u32)
        .Times(1);

    std::uint32_t data = 0;
    deserialize<std::uint32_t>(mock_deserializer, data);
}

/// deserialize<std::uint64_t>(deserializer, value) will invoke deserializer.deserialize_u64(value)
///
TEST(KingwSerde, DeserializeU64) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_u64)
        .Times(1);

    std::uint64_t data = 0;
    deserialize<std::uint64_t>(mock_deserializer, data);
}

/// deserialize<float>(deserializer, value) will invoke deserializer.deserialize_f32(value)
///
TEST(KingwSerde, DeserializeF32) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_f32)
        .Times(1);

    float data = 0.0;
    deserialize<float>(mock_deserializer, data);
}

/// deserialize<double>(deserializer, value) will invoke deserializer.deserialize_f64(value)
///
TEST(KingwSerde, DeserializeF64) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_f64)
        .Times(1);

    double data = 0.0;
    deserialize<double>(mock_deserializer, data);
}

/// deserialize<char>(deserializer, value) will invoke deserializer.deserialize_char(value)
///
TEST(KingwSerde, DeserializeChar) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_char)
        .Times(1);

    char data = '\0';
    deserialize<char>(mock_deserializer, data);
}

/// deserialize<char>(deserializer, value) will invoke deserializer.deserialize_char(value)
///
TEST(KingwSerde, DeserializeString) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_string)
        .Times(1);

    std::string data;
    deserialize<std::string>(mock_deserializer, data);
}

}  // namespace
