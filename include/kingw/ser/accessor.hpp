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
            seq.with_element(accessor(element));
        }
    }
}

template <class T>
void serialize(Serializer & serializer, const std::vector<T> & data) {
    auto seq = serializer.serialize_seq();
    for (const auto & element : data) {
        seq.with_element(accessor(element));
    }
}

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data) {
    auto map = serializer.serialize_map();
    for (const auto & kvp : data) {
        map.with_key(accessor(kvp.first));
        map.with_value(accessor(kvp.second));
    }
}

}  // namespace ser
}  // namespace kingw