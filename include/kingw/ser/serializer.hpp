#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

#include "kingw/ser/serialize.hpp"


namespace kingw {
namespace ser {

class SerializationException : public std::runtime_error {
public:
    explicit SerializationException(const std::string & message);
};

class Serializer
{
public:
    virtual ~Serializer() = default;
    virtual bool is_human_readable() const = 0;

    // Basic Types
    virtual void serialize_bool(bool value) = 0;
    virtual void serialize_i8(std::int8_t value) = 0;
    virtual void serialize_i16(std::int16_t value) = 0;
    virtual void serialize_i32(std::int32_t value) = 0;
    virtual void serialize_i64(std::int64_t value) = 0;
    virtual void serialize_u8(std::uint8_t value) = 0;
    virtual void serialize_u16(std::uint16_t value) = 0;
    virtual void serialize_u32(std::uint32_t value) = 0;
    virtual void serialize_u64(std::uint64_t value) = 0;
    virtual void serialize_f32(float value) = 0;
    virtual void serialize_f64(double value) = 0;
    virtual void serialize_char(char value) = 0;
    virtual void serialize_c_str(const char * value) = 0;
    virtual void serialize_string(const std::string & value) = 0;

    class Map
    {
    public:
        Map(Serializer & serializer);
        ~Map();
        Map & with_key(const Serialize & accessor);
        Map & with_value(const Serialize & accessor);
        void done();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual Map serialize_map();

    class Struct
    {
    public:
        Struct(Serializer & serializer);
        ~Struct();
        Struct & with_field(const Serialize & accessor, const char * name);
        void done();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual Struct serialize_struct();  

    class Seq
    {
    public:
        Seq(Serializer & serializer);
        ~Seq();
        Seq & with_element(const Serialize & accessor);
        void done();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual Seq serialize_seq();

protected:
    virtual void serialize_seq_begin() = 0;
    virtual void serialize_seq_element(const Serialize & accessor) = 0;
    virtual void serialize_seq_end() = 0;

    virtual void serialize_map_begin() = 0;
    virtual void serialize_map_key(const Serialize & accessor) = 0;
    virtual void serialize_map_value(const Serialize & accessor) = 0;
    virtual void serialize_map_end() = 0;

    virtual void serialize_struct_begin() = 0;
    virtual void serialize_struct_field(const Serialize & accessor, const char * name) = 0;
    virtual void serialize_struct_end() = 0;
};

}  // namespace ser
}  // namespace kingw

#include "kingw/ser/accessor.hpp"
