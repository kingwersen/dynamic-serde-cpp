#pragma once

#include <gmock/gmock.h>

#include <kingw/ser/serializer.hpp>


namespace kingw {
namespace ser {

class MockSerializer : public Serializer {
public:
    MOCK_METHOD(bool, is_human_readable, (), (const override));
    MOCK_METHOD(void, serialize_bool, (bool), (override));
    MOCK_METHOD(void, serialize_i8, (std::int8_t), (override));
    MOCK_METHOD(void, serialize_i16, (std::int16_t), (override));
    MOCK_METHOD(void, serialize_i32, (std::int32_t), (override));
    MOCK_METHOD(void, serialize_i64, (std::int64_t), (override));
    MOCK_METHOD(void, serialize_u8, (std::uint8_t), (override));
    MOCK_METHOD(void, serialize_u16, (std::uint16_t), (override));
    MOCK_METHOD(void, serialize_u32, (std::uint32_t), (override));
    MOCK_METHOD(void, serialize_u64, (std::uint64_t), (override));
    MOCK_METHOD(void, serialize_f32, (float), (override));
    MOCK_METHOD(void, serialize_f64, (double), (override));
    MOCK_METHOD(void, serialize_char, (char), (override));
    MOCK_METHOD(void, serialize_c_str, (const char*), (override));
    MOCK_METHOD(void, serialize_string, (const std::string &), (override));
    MOCK_METHOD(void, seq_begin, (std::size_t), (override));
    MOCK_METHOD(void, seq_serialize_element, (const Serialize &), (override));
    MOCK_METHOD(void, seq_end, (), (override));
    MOCK_METHOD(void, map_begin, (std::size_t), (override));
    MOCK_METHOD(void, map_serialize_key, (const Serialize &), (override));
    MOCK_METHOD(void, map_serialize_value, (const Serialize &), (override));
    MOCK_METHOD(void, map_serialize_entry, (const Serialize &, const Serialize &), (override));
    MOCK_METHOD(void, map_end, (), (override));
    MOCK_METHOD(void, struct_begin, (const char *, std::size_t), (override));
    MOCK_METHOD(void, struct_serialize_field, (const char *, const Serialize &), (override));
    MOCK_METHOD(void, struct_skip_field, (const char *), (override));
    MOCK_METHOD(void, struct_end, (), (override));
};

}  // namespace ser
}  // namespace kingw
