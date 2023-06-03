#pragma once

#include <cstdint>
#include <string>

#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace de {

/// @brief Default bool Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<bool>().
class BoolVisitor : public Visitor {
public:
    bool & output;
    explicit BoolVisitor(bool & output);
    const char* expecting() const override;
    void visit_bool(bool value) override;
};

/// The logic for all number-to-number conversions is similar.
/// If the value of the OTHER number is within the bounds of SELF,
/// then it's OK to copy OTHER into SELF.
/// Otherwise, throw a runtime exception.
///
/// These defines are #undef'd later.
#define KINGW_NUM_AS_SELF(TYPE, FN) void FN(TYPE value) override
#define KINGW_TRY_NUM_INTO_SELF(SELF, OTHER, FN) void FN(OTHER value) override

/// @brief Default std::int8_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::int8_t>().
class I8Visitor : public Visitor {
public:
    std::int8_t & output;
    explicit I8Visitor(std::int8_t & output);
    const char* expecting() const override;
    KINGW_NUM_AS_SELF(std::int8_t, visit_i8);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::int16_t, visit_i16);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::int64_t, visit_i64);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::uint8_t, visit_u8);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::uint16_t, visit_u16);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::int8_t, std::uint64_t, visit_u64);
};

/// @brief Default std::int16_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::int16_t>().
class I16Visitor : public Visitor {
public:
    std::int16_t & output;
    explicit I16Visitor(std::int16_t & output);
    const char* expecting() const override;
    KINGW_NUM_AS_SELF(std::int8_t, visit_i8);
    KINGW_NUM_AS_SELF(std::int16_t, visit_i16);
    KINGW_TRY_NUM_INTO_SELF(std::int16_t, std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::int16_t, std::int64_t, visit_i64);
    KINGW_TRY_NUM_INTO_SELF(std::int16_t, std::uint8_t, visit_u8);
    KINGW_TRY_NUM_INTO_SELF(std::int16_t, std::uint16_t, visit_u16);
    KINGW_TRY_NUM_INTO_SELF(std::int16_t, std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::int16_t, std::uint64_t, visit_u64);
};

/// @brief Default std::int32_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::int32_t>().
class I32Visitor : public Visitor {
public:
    std::int32_t & output;
    explicit I32Visitor(std::int32_t & output);
    const char* expecting() const override;
    KINGW_NUM_AS_SELF(std::int8_t, visit_i8);
    KINGW_NUM_AS_SELF(std::int16_t, visit_i16);
    KINGW_NUM_AS_SELF(std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::int32_t, std::int64_t, visit_i64);
    KINGW_TRY_NUM_INTO_SELF(std::int32_t, std::uint8_t, visit_u8);
    KINGW_TRY_NUM_INTO_SELF(std::int32_t, std::uint16_t, visit_u16);
    KINGW_TRY_NUM_INTO_SELF(std::int32_t, std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::int32_t, std::uint64_t, visit_u64);
};

/// @brief Default std::int64_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::int64_t>().
class I64Visitor : public Visitor {
public:
    std::int64_t & output;
    explicit I64Visitor(std::int64_t & output);
    const char* expecting() const override;
    KINGW_NUM_AS_SELF(std::int8_t, visit_i8);
    KINGW_NUM_AS_SELF(std::int16_t, visit_i16);
    KINGW_NUM_AS_SELF(std::int32_t, visit_i32);
    KINGW_NUM_AS_SELF(std::int64_t, visit_i64);
    KINGW_TRY_NUM_INTO_SELF(std::int64_t, std::uint8_t, visit_u8);
    KINGW_TRY_NUM_INTO_SELF(std::int64_t, std::uint16_t, visit_u16);
    KINGW_TRY_NUM_INTO_SELF(std::int64_t, std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::int64_t, std::uint64_t, visit_u64);
};

/// @brief Default std::uint8_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::uint8_t>().
class U8Visitor : public Visitor {
public:
    std::uint8_t & output;
    explicit U8Visitor(std::uint8_t & output);
    const char* expecting() const override;
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::int8_t, visit_i8);
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::int16_t, visit_i16);
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::int64_t, visit_i64);
    KINGW_NUM_AS_SELF(std::uint8_t, visit_u8);
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::uint16_t, visit_u16);
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::uint8_t, std::uint64_t, visit_u64);
};

/// @brief Default std::uint16_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::uint16_t>().
class U16Visitor : public Visitor {
public:
    std::uint16_t & output;
    explicit U16Visitor(std::uint16_t & output);
    const char* expecting() const override;
    KINGW_TRY_NUM_INTO_SELF(std::uint16_t, std::int8_t, visit_i8);
    KINGW_TRY_NUM_INTO_SELF(std::uint16_t, std::int16_t, visit_i16);
    KINGW_TRY_NUM_INTO_SELF(std::uint16_t, std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::uint16_t, std::int64_t, visit_i64);
    KINGW_NUM_AS_SELF(std::uint8_t, visit_u8);
    KINGW_NUM_AS_SELF(std::uint16_t, visit_u16);
    KINGW_TRY_NUM_INTO_SELF(std::uint16_t, std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::uint16_t, std::uint64_t, visit_u64);
};

/// @brief Default std::uint32_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::uint32_t>().
class U32Visitor : public Visitor {
public:
    std::uint32_t & output;
    explicit U32Visitor(std::uint32_t & output);
    const char* expecting() const override;
    KINGW_TRY_NUM_INTO_SELF(std::uint32_t, std::int8_t, visit_i8);
    KINGW_TRY_NUM_INTO_SELF(std::uint32_t, std::int16_t, visit_i16);
    KINGW_TRY_NUM_INTO_SELF(std::uint32_t, std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::uint32_t, std::int64_t, visit_i64);
    KINGW_NUM_AS_SELF(std::uint8_t, visit_u8);
    KINGW_NUM_AS_SELF(std::uint16_t, visit_u16);
    KINGW_NUM_AS_SELF(std::uint32_t, visit_u32);
    KINGW_TRY_NUM_INTO_SELF(std::uint32_t, std::uint64_t, visit_u64);
};

/// @brief Default std::uint64_t Visitor
///
/// Any visit_<integer>() is accepted, but a DeserializationException
/// will be thrown if the value is not in the integer's domain.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::uint64_t>().
class U64Visitor : public Visitor {
public:
    std::uint64_t & output;
    explicit U64Visitor(std::uint64_t & output);
    const char* expecting() const override;
    KINGW_TRY_NUM_INTO_SELF(std::uint64_t, std::int8_t, visit_i8);
    KINGW_TRY_NUM_INTO_SELF(std::uint64_t, std::int16_t, visit_i16);
    KINGW_TRY_NUM_INTO_SELF(std::uint64_t, std::int32_t, visit_i32);
    KINGW_TRY_NUM_INTO_SELF(std::uint64_t, std::int64_t, visit_i64);
    KINGW_NUM_AS_SELF(std::uint8_t, visit_u8);
    KINGW_NUM_AS_SELF(std::uint16_t, visit_u16);
    KINGW_NUM_AS_SELF(std::uint32_t, visit_u32);
    KINGW_NUM_AS_SELF(std::uint64_t, visit_u64);
};

/// @brief Default float Visitor
///
/// Any visit_<number>() is accepted.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<float>().
class F32Visitor : public Visitor {
public:
    float & output;
    explicit F32Visitor(float & output);
    const char* expecting() const override;
    KINGW_NUM_AS_SELF(float, visit_f32);
    KINGW_NUM_AS_SELF(double, visit_f64);
    KINGW_NUM_AS_SELF(std::int8_t, visit_i8);
    KINGW_NUM_AS_SELF(std::int16_t, visit_i16);
    KINGW_NUM_AS_SELF(std::int32_t, visit_i32);
    KINGW_NUM_AS_SELF(std::int64_t, visit_i64);
    KINGW_NUM_AS_SELF(std::uint8_t, visit_u8);
    KINGW_NUM_AS_SELF(std::uint16_t, visit_u16);
    KINGW_NUM_AS_SELF(std::uint32_t, visit_u32);
    KINGW_NUM_AS_SELF(std::uint64_t, visit_u64);
};

/// @brief Default double Visitor
///
/// Any visit_<number>() is accepted.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<float>().
class F64Visitor : public Visitor {
public:
    double & output;
    explicit F64Visitor(double & output);
    const char* expecting() const override;
    KINGW_NUM_AS_SELF(float, visit_f32);
    KINGW_NUM_AS_SELF(double, visit_f64);
    KINGW_NUM_AS_SELF(std::int8_t, visit_i8);
    KINGW_NUM_AS_SELF(std::int16_t, visit_i16);
    KINGW_NUM_AS_SELF(std::int32_t, visit_i32);
    KINGW_NUM_AS_SELF(std::int64_t, visit_i64);
    KINGW_NUM_AS_SELF(std::uint8_t, visit_u8);
    KINGW_NUM_AS_SELF(std::uint16_t, visit_u16);
    KINGW_NUM_AS_SELF(std::uint32_t, visit_u32);
    KINGW_NUM_AS_SELF(std::uint64_t, visit_u64);
};

#undef KINGW_TRY_NUM_INTO_SELF
#undef KINGW_NUM_AS_SELF

/// @brief Default char Visitor
///
/// Accepts visit_char(), visit_c_string(), or visit_string().
/// If the latter two, then the string must have exactly one character.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<char>().
class CharVisitor : public Visitor {
public:
    char & output;
    explicit CharVisitor(char & output);
    const char* expecting() const override;
    void visit_char(char value) override;
    void visit_c_str(const char* value, std::size_t len = -1) override;
    void visit_string(const std::string & value) override;
};

/// @brief Default std::string Visitor
///
/// Only visit_c_str() and visit_string() are accepted.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<std::string>().
class StringVisitor : public Visitor {
public:
    std::string & output;
    explicit StringVisitor(std::string & output);
    const char* expecting() const override;
    void visit_c_str(const char* value, std::size_t len = -1) override;
    void visit_string(const std::string & value) override;
};

/// @brief Default c-string Visitor
///
/// Only visit_c_str() and visit_string() are accepted.
/// @see kingw::de::Visitor for usage info.
///
/// Used in the default implementation of deserialize<char[N]>().
class CStringVisitor : public Visitor {
public:
    char* output;
    std::size_t output_len;
    explicit CStringVisitor(char* output, std::size_t output_len);
    const char* expecting() const override;
    void visit_c_str(const char* value, std::size_t len = -1) override;
    void visit_string(const std::string & value) override;
};

}  // namespace de
}  // namespace kingw
