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

    class SerializeSeq
    {
    public:
        SerializeSeq(Serializer & serializer);
        ~SerializeSeq();
        void serialize_element(const Serialize & accessor);
        void end();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual SerializeSeq serialize_seq();

    class SerializeMap
    {
    public:
        SerializeMap(Serializer & serializer);
        ~SerializeMap();
        void serialize_key(const Serialize & key);
        void serialize_value(const Serialize & value);
        void serialize_entry(const Serialize & key, const Serialize & value);
        void end();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual SerializeMap serialize_map();

    class SerializeStruct
    {
    public:
        SerializeStruct(Serializer & serializer);
        ~SerializeStruct();
        void serialize_field(const char* name, const Serialize & accessor);
        void skip_field(const char* name);
        void end();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual SerializeStruct serialize_struct();  

protected:
    virtual void seq_begin() = 0;
    virtual void seq_serialize_element(const Serialize & accessor) = 0;
    virtual void seq_end() = 0;

    virtual void map_begin() = 0;
    virtual void map_serialize_key(const Serialize & accessor) = 0;
    virtual void map_serialize_value(const Serialize & accessor) = 0;
    virtual void map_serialize_entry(const Serialize & key, const Serialize & value) = 0;
    virtual void map_end() = 0;

    virtual void struct_begin() = 0;
    virtual void struct_serialize_field(const char * name, const Serialize & accessor) = 0;
    virtual void struct_skip_field(const char * name) = 0;
    virtual void struct_end() = 0;
};

}  // namespace ser
}  // namespace kingw

#include "kingw/ser/accessor.hpp"
