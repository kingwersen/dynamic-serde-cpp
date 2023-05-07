#pragma once

#include <gmock/gmock.h>

#include <kingw/de/deserializer.hpp>


namespace kingw {
namespace de {

class MockDeserializer : public Deserializer {
public:
    MOCK_METHOD(bool, is_human_readable, (), (const override));
    MOCK_METHOD(void, deserialize_any, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_bool, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_i8, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_i16, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_i32, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_i64, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_u8, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_u16, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_u32, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_u64, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_f32, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_f64, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_char, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_string, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_seq, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_map, (Visitor &), (override));
    MOCK_METHOD(void, deserialize_struct, (const char*,
        std::initializer_list<const char*>, 
        Visitor &), (override));
};

class MockSeqAccess : public Deserializer::SeqAccess {
public:
    MOCK_METHOD(bool, has_next, (), (override));
    MOCK_METHOD(void, next_element, (Deserialize &), (override));
};

class MockMapAccess : public Deserializer::MapAccess {
public:
    MOCK_METHOD(bool, has_next, (), (override));
    MOCK_METHOD(void, next_key, (Deserialize &), (override));
    MOCK_METHOD(void, next_value, (Deserialize &), (override));
    MOCK_METHOD(void, next_entry, (Deserialize &, Deserialize &), (override));
};

class MockVisitor : public Visitor {
public:
    MOCK_METHOD(void, visit_bool, (bool), (override));
    MOCK_METHOD(void, visit_i8, (std::int8_t), (override));
    MOCK_METHOD(void, visit_i16, (std::int16_t), (override));
    MOCK_METHOD(void, visit_i32, (std::int32_t), (override));
    MOCK_METHOD(void, visit_i64, (std::int64_t), (override));
    MOCK_METHOD(void, visit_u8, (std::uint8_t), (override));
    MOCK_METHOD(void, visit_u16, (std::uint16_t), (override));
    MOCK_METHOD(void, visit_u32, (std::uint32_t), (override));
    MOCK_METHOD(void, visit_u64, (std::uint64_t), (override));
    MOCK_METHOD(void, visit_f32, (float), (override));
    MOCK_METHOD(void, visit_f64, (double), (override));
    MOCK_METHOD(void, visit_char, (char), (override));
    MOCK_METHOD(void, visit_string, (const std::string &), (override));
    MOCK_METHOD(void, visit_seq, (Deserializer::SeqAccess &), (override));
    MOCK_METHOD(void, visit_map, (Deserializer::MapAccess &), (override));
};

}  // namespace de
}  // namespace kingw
