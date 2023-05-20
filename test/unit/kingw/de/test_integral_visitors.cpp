#include <gtest/gtest.h>

#include "kingw/de/integral_visitors.hpp"

using namespace kingw::de;
using namespace testing;  // GMock, for testing::Ref, testing::_, etc.


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

TEST(KingwSerde, StringVisitorInvalid) {
    std::string output;
    StringVisitor visitor(output);

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
