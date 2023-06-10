#pragma once

#include <cstdint>
#include <stdexcept>

#include "kingw/ser/serialize.hpp"
#include "kingw/serde/string_view.hpp"


namespace kingw {
namespace ser {

class SerializationException : public std::runtime_error {
public:
    explicit SerializationException(serde::string_view message);
};

class Serializer
{
public:
    constexpr static std::size_t UNKNOWN_LENGTH = -1;

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
    virtual void serialize_string(serde::string_view value) = 0;

    class SerializeSeq
    {
    public:
        SerializeSeq(Serializer & serializer, std::size_t len);
        ~SerializeSeq();
        void serialize_element(const Serialize & accessor);
        void end();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual SerializeSeq serialize_seq(std::size_t len = UNKNOWN_LENGTH);

    class SerializeMap
    {
    public:
        SerializeMap(Serializer & serializer, std::size_t len);
        ~SerializeMap();
        void serialize_key(const Serialize & key);
        void serialize_value(const Serialize & value);
        void serialize_entry(const Serialize & key, const Serialize & value);
        void end();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual SerializeMap serialize_map(std::size_t len = UNKNOWN_LENGTH);

    class SerializeStruct
    {
    public:
        SerializeStruct(Serializer & serializer, serde::string_view name, std::size_t len);
        ~SerializeStruct();
        void serialize_field(serde::string_view name, const Serialize & accessor);
        void skip_field(serde::string_view name);
        void end();
    private:
        Serializer & serializer;
        bool closed = false;
    };
    virtual SerializeStruct serialize_struct(serde::string_view name, std::size_t len = UNKNOWN_LENGTH);

protected:
    virtual void seq_begin(std::size_t len) = 0;
    virtual void seq_serialize_element(const Serialize & accessor) = 0;
    virtual void seq_end() = 0;

    virtual void map_begin(std::size_t len) = 0;
    virtual void map_serialize_key(const Serialize & accessor) = 0;
    virtual void map_serialize_value(const Serialize & accessor) = 0;
    virtual void map_serialize_entry(const Serialize & key, const Serialize & value) = 0;
    virtual void map_end() = 0;

    virtual void struct_begin(serde::string_view name, std::size_t len) = 0;
    virtual void struct_serialize_field(serde::string_view name, const Serialize & accessor) = 0;
    virtual void struct_skip_field(serde::string_view name) = 0;
    virtual void struct_end() = 0;
};

}  // namespace ser
}  // namespace kingw

#include "kingw/ser/accessor.hpp"
