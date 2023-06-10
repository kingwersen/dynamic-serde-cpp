#include <gtest/gtest.h>

#include "kingw/de/integral_visitors.hpp"

using namespace kingw::de;
using namespace testing;  // GMock, for testing::Ref, testing::_, etc.


TEST(KingwSerde, BoolVisitorExpecting) {
    bool output = false;
    BoolVisitor visitor(output);
    EXPECT_STREQ(visitor.expecting(), "a boolean");
}

TEST(KingwSerde, BoolVisitorValid) {
    bool output = false;
    BoolVisitor visitor(output);

    EXPECT_NO_THROW(visitor.visit_bool(true));
    EXPECT_EQ(output, true);
    EXPECT_NO_THROW(visitor.visit_bool(false));
    EXPECT_EQ(output, false);
}

TEST(KingwSerde, BoolVisitorInvalid) {
    bool output = false;
    BoolVisitor visitor(output);

    EXPECT_THROW(visitor.visit_i8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_char('\0'), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_string(""), Visitor::NotImplementedException);
}

/*
TEST(KingwSerde, I8VisitorValid) {
    std::int8_t output = 0;
    I8Visitor visitor(output);

    EXPECT_NO_THROW(visitor.visit_i8(0));
    EXPECT_NO_THROW(visitor.visit_i8(std::numeric_limits<std::int8_t>::min()));
    EXPECT_NO_THROW(visitor.visit_i8(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_i16(0));
    EXPECT_NO_THROW(visitor.visit_i16(std::numeric_limits<std::int8_t>::min()));
    EXPECT_NO_THROW(visitor.visit_i16(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_i32(0));
    EXPECT_NO_THROW(visitor.visit_i32(std::numeric_limits<std::int8_t>::min()));
    EXPECT_NO_THROW(visitor.visit_i32(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_i64(0));
    EXPECT_NO_THROW(visitor.visit_i64(std::numeric_limits<std::int8_t>::min()));
    EXPECT_NO_THROW(visitor.visit_i64(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_u8(0));
    EXPECT_THROW(visitor.visit_u8(std::numeric_limits<std::int8_t>::min()), kingw::de::DeserializationException);
    EXPECT_NO_THROW(visitor.visit_u8(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_u16(0));
    EXPECT_THROW(visitor.visit_u16(std::numeric_limits<std::int8_t>::min()), kingw::de::DeserializationException);
    EXPECT_NO_THROW(visitor.visit_u16(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_u32(0));
    EXPECT_THROW(visitor.visit_u32(std::numeric_limits<std::int8_t>::min()), kingw::de::DeserializationException);
    EXPECT_NO_THROW(visitor.visit_u32(std::numeric_limits<std::int8_t>::max()));
    EXPECT_NO_THROW(visitor.visit_u64(0));
    EXPECT_THROW(visitor.visit_u64(std::numeric_limits<std::int8_t>::min()), kingw::de::DeserializationException);
    EXPECT_NO_THROW(visitor.visit_u64(std::numeric_limits<std::int8_t>::max()));
}
*/

TEST(KingwSerde, I8VisitorInvalid) {
    std::int8_t output = 0;
    I8Visitor visitor(output);

    EXPECT_THROW(visitor.visit_bool(false), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_char('\0'), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_string(""), Visitor::NotImplementedException);
}

TEST(KingwSerde, I8VisitorExpecting) {
    std::int8_t output = 0;
    I8Visitor visitor(output);
    EXPECT_STREQ(visitor.expecting(), "an 8-bit integer");
}

TEST(KingwSerde, CharVisitorExpecting) {
    char output = '\0';
    CharVisitor visitor(output);
    EXPECT_STREQ(visitor.expecting(), "a character");
}

TEST(KingwSerde, CharVisitorInvalid) {
    char output = '\0';
    CharVisitor visitor(output);

    EXPECT_THROW(visitor.visit_bool(false), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_string(""), DeserializationException);
    EXPECT_THROW(visitor.visit_string("ab"), DeserializationException);
    EXPECT_THROW(visitor.visit_string("hello"), DeserializationException);
}

TEST(KingwSerde, StringVisitorExpecting) {
    char output[8] = {};
    StringVisitor visitor(std::begin(output), std::end(output));
    EXPECT_STREQ(visitor.expecting(), "a string");
}

TEST(KingwSerde, StringVisitorInvalid) {
    char output[8] = {};
    StringVisitor visitor(std::begin(output), std::end(output));

    EXPECT_THROW(visitor.visit_bool(false), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_i64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u8(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u16(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_u64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f32(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_f64(0), Visitor::NotImplementedException);
    EXPECT_THROW(visitor.visit_char('\0'), Visitor::NotImplementedException);
}
