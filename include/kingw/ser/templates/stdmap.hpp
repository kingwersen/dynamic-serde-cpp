#pragma once

#include <map>

#include "kingw/ser/serialize.hpp"
#include "kingw/ser/serializer.hpp"


namespace kingw {
namespace ser {

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data) {
    auto map = serializer.serialize_map(data.size());
    for (const auto & kvp : data) {
        map.serialize_entry(ser::accessor(kvp.first), ser::accessor(kvp.second));
    }
    map.end();
}

template <class K, class V>
class Accessor<std::map<K, V>> : public Serialize {
public:
    explicit Accessor(const std::map<K, V> & item) : item(item) { }
    void serialize(Serializer & serializer) const override {
        ser::serialize(serializer, item);
    }
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<std::map<K, V>>();
    };
private:
    const std::map<K, V> & item;
};

template <class K, class V>
Accessor<std::map<K, V>> accessor(const std::map<K, V> & item) {
    return Accessor<std::map<K, V>>(item);
}

}  // namespace ser
}  // namespace kingw
