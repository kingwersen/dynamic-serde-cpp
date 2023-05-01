#include "kingw/de/deserializer.hpp"

#include <limits>


namespace kingw {
namespace de {

DeserializeException::DeserializeException(const std::string & message)
    : std::runtime_error(message) { }

Visitor::NotImplementedException::NotImplementedException(const std::string & message)
    : DeserializeException(message) { }

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
void Visitor::visit_seq(Deserializer::SeqAccess & value) { throw NotImplementedException("visitor unexpected type seq"); }
void Visitor::visit_map(Deserializer::MapAccess & value) { throw NotImplementedException("visitor unexpected type map"); }


template <>
void deserialize<Deserialize>(Deserializer & deserializer, Deserialize & accessor) {
    accessor.deserialize(deserializer);
}

namespace {
    class BoolVisitor : public Visitor {
    public:
        explicit BoolVisitor(bool & data) 
            : data(data) { }
        const char* expecting() const override {
            return "a boolean";
        }
        void visit_bool(bool value) override {
            data = value;
        }
    private:
        bool & data;
    };
}
template <>
void deserialize<bool>(Deserializer & deserializer, bool & data) {
    BoolVisitor visitor(data);
    deserializer.deserialize_bool(visitor);
}

// The logic for all number-to-number conversions is similar.
// If the value of the OTHER number is within the bounds of SELF,
// then it's OK to copy OTHER into SELF.
// Otherwise, throw a runtime exception.
#define NUM_AS_SELF(TYPE, FN) void FN(TYPE value) override { data = value; }
#define TRY_NUM_INTO_SELF(SELF, OTHER, FN)                      \
    void FN(OTHER value) override {                             \
        if (   value >= std::numeric_limits<SELF>::min()        \
            && value <= std::numeric_limits<SELF>::max()) {     \
            data = static_cast<SELF>(value);                    \
        } else {                                                \
            throw DeserializeException("number outside range"); \
        }                                                       \
    }

namespace {
    class I8Visitor : public Visitor {
    public:
        std::int8_t & data;
        explicit I8Visitor(std::int8_t & data) : data(data) { }
        const char* expecting() const override { return "an 8-bit integer"; }
        NUM_AS_SELF(std::int8_t, visit_i8);
        TRY_NUM_INTO_SELF(std::int8_t, std::int16_t, visit_i16);
        TRY_NUM_INTO_SELF(std::int8_t, std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::int8_t, std::int64_t, visit_i64);
        TRY_NUM_INTO_SELF(std::int8_t, std::uint8_t, visit_u8);
        TRY_NUM_INTO_SELF(std::int8_t, std::uint16_t, visit_u16);
        TRY_NUM_INTO_SELF(std::int8_t, std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::int8_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::int8_t>(Deserializer & deserializer, std::int8_t & data) {
    I8Visitor visitor(data);
    deserializer.deserialize_i8(visitor);
}

namespace {
    class I16Visitor : public Visitor {
    public:
        std::int16_t & data;
        explicit I16Visitor(std::int16_t & data) : data(data) { }
        const char* expecting() const override { return "a 16-bit integer"; }
        NUM_AS_SELF(std::int8_t, visit_i8);
        NUM_AS_SELF(std::int16_t, visit_i16);
        TRY_NUM_INTO_SELF(std::int16_t, std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::int16_t, std::int64_t, visit_i64);
        TRY_NUM_INTO_SELF(std::int16_t, std::uint8_t, visit_u8);
        TRY_NUM_INTO_SELF(std::int16_t, std::uint16_t, visit_u16);
        TRY_NUM_INTO_SELF(std::int16_t, std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::int16_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::int16_t>(Deserializer & deserializer, std::int16_t & data) {
    I16Visitor visitor(data);
    deserializer.deserialize_i16(visitor);
}

namespace {
    class I32Visitor : public Visitor {
    public:
        std::int32_t & data;
        explicit I32Visitor(std::int32_t & data) : data(data) { }
        const char* expecting() const override { return "a 32-bit integer"; }
        NUM_AS_SELF(std::int8_t, visit_i8);
        NUM_AS_SELF(std::int16_t, visit_i16);
        NUM_AS_SELF(std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::int32_t, std::int64_t, visit_i64);
        TRY_NUM_INTO_SELF(std::int32_t, std::uint8_t, visit_u8);
        TRY_NUM_INTO_SELF(std::int32_t, std::uint16_t, visit_u16);
        TRY_NUM_INTO_SELF(std::int32_t, std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::int32_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::int32_t>(Deserializer & deserializer, std::int32_t & data) {
    I32Visitor visitor(data);
    deserializer.deserialize_i32(visitor);
}

namespace {
    class I64Visitor : public Visitor {
    public:
        std::int64_t & data;
        explicit I64Visitor(std::int64_t & data) : data(data) { }
        const char* expecting() const override { return "a 64-bit integer"; }
        NUM_AS_SELF(std::int8_t, visit_i8);
        NUM_AS_SELF(std::int16_t, visit_i16);
        NUM_AS_SELF(std::int32_t, visit_i32);
        NUM_AS_SELF(std::int64_t, visit_i64);
        TRY_NUM_INTO_SELF(std::int64_t, std::uint8_t, visit_u8);
        TRY_NUM_INTO_SELF(std::int64_t, std::uint16_t, visit_u16);
        TRY_NUM_INTO_SELF(std::int64_t, std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::int64_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::int64_t>(Deserializer & deserializer, std::int64_t & data) {
    I64Visitor visitor(data);
    deserializer.deserialize_i64(visitor);
}

namespace {
    class U8Visitor : public Visitor {
    public:
        std::uint8_t & data;
        explicit U8Visitor(std::uint8_t & data) : data(data) { }
        const char* expecting() const override { return "an 8-bit unsigned integer"; }
        TRY_NUM_INTO_SELF(std::uint8_t, std::int8_t, visit_i8);
        TRY_NUM_INTO_SELF(std::uint8_t, std::int16_t, visit_i16);
        TRY_NUM_INTO_SELF(std::uint8_t, std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::uint8_t, std::int64_t, visit_i64);
        NUM_AS_SELF(std::uint8_t, visit_u8);
        TRY_NUM_INTO_SELF(std::uint8_t, std::uint16_t, visit_u16);
        TRY_NUM_INTO_SELF(std::uint8_t, std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::uint8_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::uint8_t>(Deserializer & deserializer, std::uint8_t & data) {
    U8Visitor visitor(data);
    deserializer.deserialize_u8(visitor);
}

namespace {
    class U16Visitor : public Visitor {
    public:
        std::uint16_t & data;
        explicit U16Visitor(std::uint16_t & data) : data(data) { }
        const char* expecting() const override { return "a 16-bit unsigned integer"; }
        TRY_NUM_INTO_SELF(std::uint16_t, std::int8_t, visit_i8);
        TRY_NUM_INTO_SELF(std::uint16_t, std::int16_t, visit_i16);
        TRY_NUM_INTO_SELF(std::uint16_t, std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::uint16_t, std::int64_t, visit_i64);
        NUM_AS_SELF(std::uint8_t, visit_u8);
        NUM_AS_SELF(std::uint16_t, visit_u16);
        TRY_NUM_INTO_SELF(std::uint16_t, std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::uint16_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::uint16_t>(Deserializer & deserializer, std::uint16_t & data) {
    U16Visitor visitor(data);
    deserializer.deserialize_u16(visitor);
}

namespace {
    class U32Visitor : public Visitor {
    public:
        std::uint32_t & data;
        explicit U32Visitor(std::uint32_t & data) : data(data) { }
        const char* expecting() const override { return "a 32-bit unsigned integer"; }
        TRY_NUM_INTO_SELF(std::uint32_t, std::int8_t, visit_i8);
        TRY_NUM_INTO_SELF(std::uint32_t, std::int16_t, visit_i16);
        TRY_NUM_INTO_SELF(std::uint32_t, std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::uint32_t, std::int64_t, visit_i64);
        NUM_AS_SELF(std::uint8_t, visit_u8);
        NUM_AS_SELF(std::uint16_t, visit_u16);
        NUM_AS_SELF(std::uint32_t, visit_u32);
        TRY_NUM_INTO_SELF(std::uint32_t, std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::uint32_t>(Deserializer & deserializer, std::uint32_t & data) {
    U32Visitor visitor(data);
    deserializer.deserialize_u32(visitor);
}

namespace {
    class U64Visitor : public Visitor {
    public:
        std::uint64_t & data;
        explicit U64Visitor(std::uint64_t & data) : data(data) { }
        const char* expecting() const override { return "a 64-bit unsigned integer"; }
        TRY_NUM_INTO_SELF(std::uint64_t, std::int8_t, visit_i8);
        TRY_NUM_INTO_SELF(std::uint64_t, std::int16_t, visit_i16);
        TRY_NUM_INTO_SELF(std::uint64_t, std::int32_t, visit_i32);
        TRY_NUM_INTO_SELF(std::uint64_t, std::int64_t, visit_i64);
        NUM_AS_SELF(std::uint8_t, visit_u8);
        NUM_AS_SELF(std::uint16_t, visit_u16);
        NUM_AS_SELF(std::uint32_t, visit_u32);
        NUM_AS_SELF(std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<std::uint64_t>(Deserializer & deserializer, std::uint64_t & data) {
    U64Visitor visitor(data);
    deserializer.deserialize_u64(visitor);
}

namespace {
    class F32Visitor : public Visitor {
    public:
        float & data;
        explicit F32Visitor(float & data) : data(data) { }
        const char* expecting() const override { return "a 32-bit float"; }
        NUM_AS_SELF(float, visit_f32);
        NUM_AS_SELF(double, visit_f64);
        NUM_AS_SELF(std::int8_t, visit_i8);
        NUM_AS_SELF(std::int16_t, visit_i16);
        NUM_AS_SELF(std::int32_t, visit_i32);
        NUM_AS_SELF(std::int64_t, visit_i64);
        NUM_AS_SELF(std::uint8_t, visit_u8);
        NUM_AS_SELF(std::uint16_t, visit_u16);
        NUM_AS_SELF(std::uint32_t, visit_u32);
        NUM_AS_SELF(std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<float>(Deserializer & deserializer, float & data) {
    F32Visitor visitor(data);
    deserializer.deserialize_f32(visitor);
}

namespace {
    class F64Visitor : public Visitor {
    public:
        double & data;
        explicit F64Visitor(double & data) : data(data) { }
        const char* expecting() const override { return "a 64-bit float"; }
        NUM_AS_SELF(float, visit_f32);
        NUM_AS_SELF(double, visit_f64);
        NUM_AS_SELF(std::int8_t, visit_i8);
        NUM_AS_SELF(std::int16_t, visit_i16);
        NUM_AS_SELF(std::int32_t, visit_i32);
        NUM_AS_SELF(std::int64_t, visit_i64);
        NUM_AS_SELF(std::uint8_t, visit_u8);
        NUM_AS_SELF(std::uint16_t, visit_u16);
        NUM_AS_SELF(std::uint32_t, visit_u32);
        NUM_AS_SELF(std::uint64_t, visit_u64);
    };
}
template <>
void deserialize<double>(Deserializer & deserializer, double & data) {
    F64Visitor visitor(data);
    deserializer.deserialize_f64(visitor);
}

namespace {
    class CharVisitor : public Visitor {
    public:
        char & data;
        explicit CharVisitor(char & data) : data(data) { }
        const char* expecting() const override { return "a character"; }
        void visit_char(char value) override {
            data = value;
        }
        void visit_string(const std::string & value) override {
            if (value.size() == 1) {
                data = value[0];
            } else {
                throw DeserializeException("string does not contain exactly one character");
            }
        }
    };
}
template <>
void deserialize<char>(Deserializer & deserializer, char & data) {
    CharVisitor visitor(data);
    deserializer.deserialize_char(visitor);
}

namespace {
    class StringVisitor : public Visitor {
    public:
        std::string & data;
        explicit StringVisitor(std::string & data) : data(data) { }
        const char* expecting() const override { return "a string"; }
        void visit_string(const std::string & value) override {
            data = value;
        }
    };
}
template <>
void deserialize<std::string>(Deserializer & deserializer, std::string & data) {
    StringVisitor visitor(data);
    deserializer.deserialize_string(visitor);
}

}  // namespace de
}  // namespace kingw
