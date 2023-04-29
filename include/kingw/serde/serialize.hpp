#pragma once

#include <stdexcept>
#include <vector>
#include <map>

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace ser {

class SerializationFailureException : public std::runtime_error {
public:
    SerializationFailureException(const std::string & str) : std::runtime_error(str) { }
};

class SerializeNotImplementedException : public SerializationFailureException {
public:
    SerializeNotImplementedException(const std::string & str) : SerializationFailureException(str) { }
};


class Serializer;

class Dynamic
{
public:
    virtual ~Dynamic() = default;
    virtual void serialize(Serializer & serializer) const = 0;
    virtual TypeTraits traits() const = 0;
};

class Serializer
{
public:
    virtual ~Serializer() = default;

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

    // Lists/Sequences
    class Seq
    {
    public:
        Seq(Serializer & serializer);
        ~Seq();
        void serialize_element(const Dynamic & accessor);
    private:
        Serializer & serializer;
    };
    virtual Seq serialize_seq();
protected:
    virtual void serialize_seq_begin() = 0;
    virtual void serialize_seq_element(const Dynamic & accessor) = 0;
    virtual void serialize_seq_end() = 0;
public:

    // Maps
    class Map
    {
    public:
        Map(Serializer & serializer);
        ~Map();
        void serialize_key(const Dynamic & accessor);
        void serialize_value(const Dynamic & accessor);
    private:
        Serializer & serializer;
    };
    virtual Map serialize_map();
protected:
    virtual void serialize_map_begin() = 0;
    virtual void serialize_map_key(const Dynamic & accessor) = 0;
    virtual void serialize_map_value(const Dynamic & accessor) = 0;
    virtual void serialize_map_end() = 0;
public:

    // Structs
    class Struct
    {
    public:
        Struct(Serializer & serializer);
        ~Struct();
        void serialize_field(const Dynamic & accessor, const char * name);
    private:
        Serializer & serializer;
    };
    virtual Struct serialize_struct();
protected:
    virtual void serialize_struct_begin() = 0;
    virtual void serialize_struct_field(const Dynamic & accessor, const char * name) = 0;
    virtual void serialize_struct_end() = 0;
};


template <class T>
void serialize(Serializer & serializer, const T & data) {
    throw SerializeNotImplementedException(
        std::string("serialize() not implemented for type '") + typeid(T).name() + "'");
}

template <class T, std::size_t N>
void serialize(Serializer & serializer, const T (&data)[N]);

template <class T>
void serialize(Serializer & serializer, const std::vector<T> & data);

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data);


template <class T>
class Accessor :
    public Dynamic
{
public:
    explicit Accessor(const T & item) : item(item) {}
    void serialize(Serializer & serializer) const override {
        ser::serialize(serializer, item);
    }
    TypeTraits traits() const override {
        return TypeTraits::of<T>();
    };

private:
    const T & item;
};

template <class T>
Accessor<T> accessor(const T & item) {
    return Accessor<T>(item);
}

template <class T, std::size_t N>
void serialize(Serializer & serializer, const T (&data)[N]) {
    if (std::is_same<T, char>::value) {
        serializer.serialize_c_str(data);
    } else {
        auto seq = serializer.serialize_seq();
        for (const auto & element : data) {
            seq.serialize_element(accessor(element));
        }
    }
}

template <class T>
void serialize(Serializer & serializer, const std::vector<T> & data) {
    auto seq = serializer.serialize_seq();
    for (const auto & element : data) {
        seq.serialize_element(accessor(element));
    }
}

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data) {
    auto map = serializer.serialize_map();
    for (const auto & kvp : data) {
        map.serialize_key(accessor(kvp.first));
        map.serialize_value(accessor(kvp.second));
    }
}

}  // namespace ser
}  // namespace kingw
