#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

#include "kingw/serde/dynamic.hpp"


namespace kingw {
namespace ser {

class DeserializeException : public std::runtime_error {
public:
    explicit DeserializeException(const std::string & message);
};

class Visitor;

class Deserializer
{
public:
    virtual ~Deserializer() = default;
    virtual bool is_human_readable() const = 0;

    // Basic Types
    virtual void deserialize_any(Visitor & visitor) = 0;
    virtual void deserialize_bool(Visitor & visitor) = 0;
    virtual void deserialize_i8(Visitor & visitor) = 0;
    virtual void deserialize_i16(Visitor & visitor) = 0;
    virtual void deserialize_i32(Visitor & visitor) = 0;
    virtual void deserialize_i64(Visitor & visitor) = 0;
    virtual void deserialize_u8(Visitor & visitor) = 0;
    virtual void deserialize_u16(Visitor & visitor) = 0;
    virtual void deserialize_u32(Visitor & visitor) = 0;
    virtual void deserialize_u64(Visitor & visitor) = 0;
    virtual void deserialize_f32(Visitor & visitor) = 0;
    virtual void deserialize_f64(Visitor & visitor) = 0;
    virtual void deserialize_char(Visitor & visitor) = 0;
    virtual void deserialize_string(Visitor & visitor) = 0;
    virtual void deserialize_seq(Visitor & visitor) = 0;
    virtual void deserialize_map(Visitor & visitor) = 0;
    //virtual void deserialize_struct(Visitor & visitor) = 0;

    class SeqAccess
    {
    public:
        virtual ~SeqAccess() = default;
        virtual bool has_next() = 0;
        virtual void next_element(Dynamic & element) = 0;
    };

    class MapAccess
    {
    public:
        virtual ~MapAccess() = default;
        virtual bool has_next() = 0;
        virtual void next_key(Dynamic & key) = 0;
        virtual void next_value(Dynamic & value) = 0;
        virtual void next_entry(Dynamic & key, Dynamic & value) = 0;
    };
};

class Visitor
{
public:
    class NotImplementedException : public DeserializeException {
    public:
        explicit NotImplementedException(const std::string & message);
    };

    virtual ~Visitor() = default;

    virtual const char* expecting() const = 0;
    virtual void visit_bool(bool value);
    virtual void visit_i8(std::int8_t value);
    virtual void visit_i16(std::int16_t value);
    virtual void visit_i32(std::int32_t value);
    virtual void visit_i64(std::int64_t value);
    virtual void visit_u8(std::uint8_t value);
    virtual void visit_u16(std::uint16_t value);
    virtual void visit_u32(std::uint32_t value);
    virtual void visit_u64(std::uint64_t value);
    virtual void visit_f32(float value);
    virtual void visit_f64(double value);
    virtual void visit_char(char value);
    virtual void visit_string(const std::string & value);
    virtual void visit_seq(Deserializer::SeqAccess & value);
    virtual void visit_map(Deserializer::MapAccess & value);
};

template <class T>
void deserialize(Deserializer & deserializer, T & data);

}  // namespace ser
}  // namespace kingw
