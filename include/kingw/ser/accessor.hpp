#pragma once

#include <vector>
#include <map>

#include "kingw/ser/serialize.hpp"
#include "kingw/ser/serializer.hpp"


namespace kingw {
namespace ser {

template <class T, std::size_t N>
void serialize(Serializer & serializer, const T (&data)[N]);

template <class T>
void serialize(Serializer & serializer, const std::vector<T> & data);

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data);

template <class T>
class Accessor : public Serialize {
public:
    explicit Accessor(const T & item) : item(item) { }
    void serialize(Serializer & serializer) const override {
        ser::serialize(serializer, item);
    }
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<T>();
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
            seq.serialize_element(ser::accessor(element));
        }
        seq.end();
    }
}

template <class T>
void serialize(Serializer & serializer, const std::vector<T> & data) {
    auto seq = serializer.serialize_seq();
    for (const auto & element : data) {
        seq.serialize_element(ser::accessor(element));
    }
    seq.end();
}

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data) {
    auto map = serializer.serialize_map();
    for (const auto & kvp : data) {
        map.serialize_entry(ser::accessor(kvp.first), ser::accessor(kvp.second));
    }
    map.end();
}

}  // namespace ser
}  // namespace kingw
