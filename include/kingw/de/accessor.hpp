#pragma once

#include <vector>
#include <map>

#include "kingw/de/deserialize.hpp"
#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace de {

template <class T, std::size_t N>
void deserialize(Deserializer & deserializer, T (&data)[N]);

template <class T>
void deserialize(Deserializer & deserializer, std::vector<T> & data);

template <class K, class V>
void deserialize(Deserializer & deserializer, std::map<K, V> & data);

template <class T>
class Accessor : public de::Deserialize
{
public:
    explicit Accessor(T & item) : item(item) {}
    void deserialize(Deserializer & serializer) override {
        de::deserialize(serializer, item);
    }
    TypeTraits traits() const override {
        return TypeTraits::of<T>();
    };
private:
    T & item;
};

template <class T>
Accessor<T> accessor(T & item) {
    return Accessor<T>(item);
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
            auto accessor = de::accessor(data[count++]);
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
            auto key_accessor = de::accessor(key);
            auto value_accessor = de::accessor(value);
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

}  // namespace de
}  // namespace kingw
