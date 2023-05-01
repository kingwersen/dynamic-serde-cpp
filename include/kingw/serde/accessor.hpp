#pragma once

#include <vector>
#include <map>

#include "kingw/serde/serialize.hpp"
#include "kingw/serde/deserialize.hpp"


namespace kingw {
namespace ser {

template <class T, std::size_t N>
void serialize(Serializer & serializer, const T (&data)[N]);

template <class T>
void serialize(Serializer & serializer, const std::vector<T> & data);

template <class K, class V>
void serialize(Serializer & serializer, const std::map<K, V> & data);

template <class T, std::size_t N>
void deserialize(Deserializer & deserializer, T (&data)[N]);

template <class T>
void deserialize(Deserializer & deserializer, std::vector<T> & data);

template <class K, class V>
void deserialize(Deserializer & deserializer, std::map<K, V> & data);

template <class T>
class ConstAccessor :
    public ser::Dynamic
{
public:
    explicit ConstAccessor(const T & item) : item(item) {}
    void serialize(Serializer & serializer) const override {
        ser::serialize(serializer, item);
    }
    TypeTraits traits() const override {
        return TypeTraits::of<T>();
    };

protected:
    void deserialize(Deserializer & serializer) override {}
    const T & item;
};

template <class T>
class Accessor :
    public ConstAccessor<T>
{
public:
    explicit Accessor(T & item) : ConstAccessor<T>(item) {}
    void deserialize(Deserializer & serializer) override {
        ser::deserialize(serializer, const_cast<T&>(this->item));
    }
};

template <class T>
ConstAccessor<T> accessor(const T & item) {
    return ConstAccessor<T>(item);
}

template <class T>
Accessor<T> accessor(T & item) {
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

template <class T, std::size_t N>
void deserialize(Deserializer & deserializer, T (&data)[N]) {
    // TODO
    throw std::logic_error("deserialize() fpr C arrays not yet implemented");
}

template <class T>
class StdVectorVisitor : public Visitor {
public:
    std::vector<T> & data;
    explicit StdVectorVisitor(std::vector<T> & data) : data(data) { }
    const char* expecting() const override { return "a sequence of items"; }
    void visit_seq(Deserializer::SeqAccess & seq) override {
        std::size_t count = 0;
        while (seq.has_next()) {
            data.push_back({});
            auto accessor = ser::accessor(data[count++]);
            seq.next_element(accessor);
        }
    }
};
template <class T>
void deserialize(Deserializer & deserializer, std::vector<T> & data) {
    StdVectorVisitor<T> visitor(data);
    deserializer.deserialize_seq(visitor);
}

template <class K, class V>
class StdMapVisitor : public Visitor {
public:
    std::map<K, V> & data;
    explicit StdMapVisitor(std::map<K, V> & data) : data(data) { }
    const char* expecting() const override { return "a map of items"; }
    void visit_map(Deserializer::MapAccess & map) override {
        std::size_t count = 0;
        while (map.has_next()) {
            K key{};
            V value{};
            auto key_accessor = ser::accessor(key);
            auto value_accessor = ser::accessor(value);
            map.next_entry(key_accessor, value_accessor);
            data[std::move(key)] = std::move(value);
        }
    }
};
template <class K, class V>
void deserialize(Deserializer & deserializer, std::map<K, V> & data) {
    StdMapVisitor<K, V> visitor(data);
    deserializer.deserialize_map(visitor);
}

}  // namespace ser
}  // namespace kingw
