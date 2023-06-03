#include <limits>

#include <gmock/gmock.h>

#include "kingw/mock/ser/mock_serialize.hpp"
#include "kingw/mock/ser/mock_serializer.hpp"

using namespace kingw::ser;
using namespace testing;  // GMock, for testing::Ref, testing::_, etc.


namespace {

/// SerializeSeq shall call serializer.seq_begin() on construction, serializer.seq_end() on destruction,
/// and serializer.serialize_element() once for every element provided.
TEST(KingwSerde, SerializeSeq) {
    MockSerialize mock_serialize1;
    MockSerialize mock_serialize2;
    MockSerialize mock_serialize3;
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, seq_begin(_)).Times(1);
    EXPECT_CALL(mock_serializer, seq_serialize_element(Ref(mock_serialize1))).Times(1);
    EXPECT_CALL(mock_serializer, seq_serialize_element(Ref(mock_serialize2))).Times(1);
    EXPECT_CALL(mock_serializer, seq_serialize_element(Ref(mock_serialize3))).Times(1);
    EXPECT_CALL(mock_serializer, seq_end()).Times(1);

    auto seq = mock_serializer.serialize_seq();
    seq.serialize_element(mock_serialize1);
    seq.serialize_element(mock_serialize2);
    seq.serialize_element(mock_serialize3);
}

/// SerializeSeq shall only always invoke serializer.seq_end() exactly once.
/// If end() is called manually, then seq_end() will not be called again on repeat or at destruction.
TEST(KingwSerde, SerializeSeqEnd) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, seq_end).Times(1);

    auto seq = mock_serializer.serialize_seq();
    seq.end();
    seq.end();
}

/// SerializeMap shall call serializer.seq_begin() on construction, serializer.seq_end() on destruction,
/// and serializer.serialize_key(), .serialize_value(), or .serialize_entry() once for each call.
/// SerializeMap::serialize_entry() does *NOT* shortcut to calling key() and then value().
TEST(KingwSerde, SerializeMap) {
    MockSerialize mock_serialize1;
    MockSerialize mock_serialize2;
    MockSerialize mock_serialize3;
    MockSerialize mock_serialize4;
    MockSerialize mock_serialize5;
    MockSerialize mock_serialize6;
    MockSerialize mock_serialize7;
    MockSerialize mock_serialize8;
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, map_begin(_)).Times(1);
    EXPECT_CALL(mock_serializer, map_serialize_key(Ref(mock_serialize1))).Times(1);
    EXPECT_CALL(mock_serializer, map_serialize_value(Ref(mock_serialize2))).Times(1);
    EXPECT_CALL(mock_serializer, map_serialize_entry(Ref(mock_serialize3), Ref(mock_serialize4))).Times(1);
    EXPECT_CALL(mock_serializer, map_serialize_key(Ref(mock_serialize5))).Times(1);
    EXPECT_CALL(mock_serializer, map_serialize_value(Ref(mock_serialize6))).Times(1);
    EXPECT_CALL(mock_serializer, map_serialize_entry(Ref(mock_serialize7), Ref(mock_serialize8))).Times(1);
    EXPECT_CALL(mock_serializer, map_end()).Times(1);

    auto map = mock_serializer.serialize_map();
    map.serialize_key(mock_serialize1);
    map.serialize_value(mock_serialize2);
    map.serialize_entry(mock_serialize3, mock_serialize4);
    map.serialize_key(mock_serialize5);
    map.serialize_value(mock_serialize6);
    map.serialize_entry(mock_serialize7, mock_serialize8);
}

/// SerializeMap shall only always invoke serializer.map_end() exactly once.
/// If end() is called manually, then map_end() will not be called again on repeat or at destruction.
TEST(KingwSerde, SerializeMapEnd) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, map_end).Times(1);

    auto map = mock_serializer.serialize_map();
    map.end();
    map.end();
}

/// SerializeStruct shall call serializer.struct_begin() on construction, serializer.struct_end() on destruction,
/// and serializer.struct_serialize_field() or serializer.struct_skip_field() once for each call.
TEST(KingwSerde, SerializeStruct) {
    const char* name1 = "name1";
    const char* name2 = "name2";
    const char* name3 = "name3";
    const char* name4 = "name4";
    MockSerialize mock_serialize1;
    MockSerialize mock_serialize2;
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, struct_begin(_, _)).Times(1);
    EXPECT_CALL(mock_serializer, struct_serialize_field(name1, Ref(mock_serialize1))).Times(1);
    EXPECT_CALL(mock_serializer, struct_serialize_field(name2, Ref(mock_serialize2))).Times(1);
    EXPECT_CALL(mock_serializer, struct_skip_field(name3)).Times(1);
    EXPECT_CALL(mock_serializer, struct_skip_field(name4)).Times(1);
    EXPECT_CALL(mock_serializer, struct_end()).Times(1);

    auto state = mock_serializer.serialize_struct("example");
    state.serialize_field(name1, mock_serialize1);
    state.serialize_field(name2, mock_serialize2);
    state.skip_field(name3);
    state.skip_field(name4);
}

/// SerializeStruct shall only always invoke serializer.struct_end() exactly once.
/// If end() is called manually, then struct_end() will not be called again on repeat or at destruction.
TEST(KingwSerde, SerializeStructEnd) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, struct_end).Times(1);

    auto state = mock_serializer.serialize_struct("example");
    state.end();
    state.end();
}

/// serialize<bool>(serializer, value) will invoke serializer.serialize_bool(value)
///
TEST(KingwSerde, SerializeBool) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_bool)
        .Times(2);

    serialize<bool>(mock_serializer, true);
    serialize<bool>(mock_serializer, false);
}

/// serialize<std::int8_t>(serializer, value) will invoke serializer.serialize_i8(value)
///
TEST(KingwSerde, SerializeI8) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_i8)
        .Times(5);

    serialize<std::int8_t>(mock_serializer, std::numeric_limits<std::uint8_t>::min());
    serialize<std::int8_t>(mock_serializer, -1);
    serialize<std::int8_t>(mock_serializer, 0);
    serialize<std::int8_t>(mock_serializer, 1);
    serialize<std::int8_t>(mock_serializer, std::numeric_limits<std::uint8_t>::max());
}

/// serialize<std::int16_t>(serializer, value) will invoke serializer.serialize_i16(value)
///
TEST(KingwSerde, SerializeI16) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_i16)
        .Times(5);

    serialize<std::int16_t>(mock_serializer, std::numeric_limits<std::int16_t>::min());
    serialize<std::int16_t>(mock_serializer, -1);
    serialize<std::int16_t>(mock_serializer, 0);
    serialize<std::int16_t>(mock_serializer, 1);
    serialize<std::int16_t>(mock_serializer, std::numeric_limits<std::int16_t>::max());
}

/// serialize<std::int32_t>(serializer, value) will invoke serializer.serialize_i32(value)
///
TEST(KingwSerde, SerializeI32) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_i32)
        .Times(5);

    serialize<std::int32_t>(mock_serializer, std::numeric_limits<std::int32_t>::min());
    serialize<std::int32_t>(mock_serializer, -1);
    serialize<std::int32_t>(mock_serializer, 0);
    serialize<std::int32_t>(mock_serializer, 1);
    serialize<std::int32_t>(mock_serializer, std::numeric_limits<std::int32_t>::max());
}

/// serialize<std::int64_t>(serializer, value) will invoke serializer.serialize_i64(value)
///
TEST(KingwSerde, SerializeI64) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_i64)
        .Times(5);

    serialize<std::int64_t>(mock_serializer, std::numeric_limits<std::int64_t>::min());
    serialize<std::int64_t>(mock_serializer, -1);
    serialize<std::int64_t>(mock_serializer, 0);
    serialize<std::int64_t>(mock_serializer, 1);
    serialize<std::int64_t>(mock_serializer, std::numeric_limits<std::int64_t>::max());
}

/// serialize<std::uint8_t>(serializer, value) will invoke serializer.serialize_u8(value)
///
TEST(KingwSerde, SerializeU8) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_u8)
        .Times(3);

    serialize<std::uint8_t>(mock_serializer, 0);
    serialize<std::uint8_t>(mock_serializer, 1);
    serialize<std::uint8_t>(mock_serializer, std::numeric_limits<std::uint8_t>::max());
}

/// serialize<std::uint16_t>(serializer, value) will invoke serializer.serialize_u16(value)
///
TEST(KingwSerde, SerializeU16) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_u16)
        .Times(3);

    serialize<std::uint16_t>(mock_serializer, 0);
    serialize<std::uint16_t>(mock_serializer, 1);
    serialize<std::uint16_t>(mock_serializer, std::numeric_limits<std::uint16_t>::max());
}

/// serialize<std::uint32_t>(serializer, value) will invoke serializer.serialize_u32(value)
///
TEST(KingwSerde, SerializeU32) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_u32)
        .Times(3);

    serialize<std::uint32_t>(mock_serializer, 0);
    serialize<std::uint32_t>(mock_serializer, 1);
    serialize<std::uint32_t>(mock_serializer, std::numeric_limits<std::uint32_t>::max());
}

/// serialize<std::uint64_t>(serializer, value) will invoke serializer.serialize_u64(value)
///
TEST(KingwSerde, SerializeU64) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_u64)
        .Times(3);

    serialize<std::uint64_t>(mock_serializer, 0);
    serialize<std::uint64_t>(mock_serializer, 1);
    serialize<std::uint64_t>(mock_serializer, std::numeric_limits<std::uint64_t>::max());
}

/// serialize<float>(serializer, value) will invoke serializer.serialize_f32(value)
///
TEST(KingwSerde, SerializeF32) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_f32)
        .Times(5);

    serialize<float>(mock_serializer, std::numeric_limits<float>::min());
    serialize<float>(mock_serializer, -1.0);
    serialize<float>(mock_serializer, 0.0);
    serialize<float>(mock_serializer, 1.0);
    serialize<float>(mock_serializer, std::numeric_limits<float>::max());
}

/// serialize<double>(serializer, value) will invoke serializer.serialize_f64(value)
///
TEST(KingwSerde, SerializeF64) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_f64)
        .Times(5);

    serialize<double>(mock_serializer, std::numeric_limits<double>::min());
    serialize<double>(mock_serializer, -1.0);
    serialize<double>(mock_serializer, 0.0);
    serialize<double>(mock_serializer, 1.0);
    serialize<double>(mock_serializer, std::numeric_limits<double>::max());
}

/// serialize<char>(serializer, value) will invoke serializer.serialize_char(value)
///
TEST(KingwSerde, SerializeChar) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_char)
        .Times(8);

    serialize<char>(mock_serializer, std::numeric_limits<char>::min());
    serialize<char>(mock_serializer, '\n');
    serialize<char>(mock_serializer, '\r');
    serialize<char>(mock_serializer, '\0');
    serialize<char>(mock_serializer, 'a');
    serialize<char>(mock_serializer, 'A');
    serialize<char>(mock_serializer, '1');
    serialize<char>(mock_serializer, std::numeric_limits<char>::max());
}

/// serialize<char>(serializer, value) will invoke serializer.serialize_char(value)
///
TEST(KingwSerde, SerializeString) {
    MockSerializer mock_serializer;
    EXPECT_CALL(mock_serializer, serialize_string)
        .Times(3);

    serialize<std::string>(mock_serializer, "");
    serialize<std::string>(mock_serializer, "Hello");
    serialize<std::string>(mock_serializer, "World");
}

}  // namespace
