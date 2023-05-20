#include <gmock/gmock.h>

#include "kingw/mock/de/mock_deserializer.hpp"

using namespace kingw::de;
using namespace testing;  // GMock, for testing::Ref, testing::_, etc.


TEST(KingwSerde, BoolVisitorValid) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_bool)
        .Times(2)
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_bool(true); } ]))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_bool(false); } ]));

    bool output = false;
    EXPECT_NO_THROW(deserialize<bool>(mock_deserializer, output));
    EXPECT_EQ(output, true);
    EXPECT_NO_THROW(deserialize<bool>(mock_deserializer, output));
    EXPECT_EQ(output, false);
}

TEST(KingwSerde, BoolVisitorInvalid) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_bool)
        .Times(12)
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i8(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i16(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u8(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u16(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_f32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_f64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_char('\0'); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_string(""); } ));

    bool output = false;
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<bool>(mock_deserializer, output), Visitor::NotImplementedException);
}

TEST(KingwSerde, CharVisitorInvalid) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_char)
        .Times(11)
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_bool(false); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i8(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i16(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u8(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u16(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_f32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_f64(0); } ));

    char output = '\0';
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<char>(mock_deserializer, output), Visitor::NotImplementedException);
}

TEST(KingwSerde, StringVisitorInvalid) {
    MockDeserializer mock_deserializer;
    EXPECT_CALL(mock_deserializer, deserialize_string)
        .Times(11)
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_bool(false); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i8(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i16(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_i64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u8(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u16(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_u64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_f32(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_f64(0); } ))
        .WillOnce(Invoke([] (Visitor & visitor) { visitor.visit_char('\0'); } ));

    std::string output;
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
    EXPECT_THROW(deserialize<std::string>(mock_deserializer, output), Visitor::NotImplementedException);
}
