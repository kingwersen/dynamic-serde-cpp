#pragma once

#include <map>

#include "kingw/de/deserialize.hpp"
#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace de {

/// @brief Generic Visitor class for `std::map<K, V>` deserialization
///
/// Used by `deserialize<std::map<K, V>>()`.
/// You probably won't manually use this yourself, but you can if you need to.
///
/// @tparam K Map key type
/// @tparam V Map value type
template <class K, class V>
class StdMapVisitor : public Visitor {
public:
    /// @brief StdMapVisitor Constructor
    /// @param output Reference of variable to deserialize into
    explicit StdMapVisitor(std::map<K, V> & output) : output(output) { }

    /// @brief Explanation of what this Visitor is expecting
    /// @return A string literal
    const char* expecting() const override { return "a map of items"; }

    /// @brief Extract items from `map` and put them into `output`
    ///
    /// `Deserializer` does not know what type of map we are.
    ///
    /// First, by calling `Deserializer::deserialize_map()` we tell the
    /// `Deserializer` that the next item it will deserialize should
    /// be some sort of map/structure. `map` is a generic, untyped
    /// representation of the map that it found.
    ///
    /// It is our job in this function to figure out what it found
    /// and convert it into a `std::map<K, V>`.
    ///
    /// We do this by providing `Accessor<T>`s (containing references
    /// to a new key-value pair in our map) back to the `Deserializer`
    /// through `map.next_entry()`. The `Deserializer` can
    /// then call `accessor.deserialize()` on both the key and the
    /// value accessors to fill the data.
    ///
    /// @param seq Data from `Deserializer`
    void visit_map(Deserializer::MapAccess & map) override {
        while (map.has_next()) {
            K key{};
            V value{};
            de::Accessor<K> key_accessor(key);
            de::Accessor<V> value_accessor(value);
            map.next_entry(key_accessor, value_accessor);
            output[std::move(key)] = std::move(value);
        }
    }

private:
    /// @brief Reference of variable to deserialize into
    std::map<K, V> & output;
};

/// @brief `deserialize()` specialization for std::map.
///
/// Since `std::map` is a template, this definition has to be
/// available anywhere a `std::map` is deserialized.
///
/// @tparam K Map key type
/// @tparam V Map value type
/// @param deserializer Deserializer to extract from
/// @param output Output location
template <class K, class V>
void deserialize(Deserializer & deserializer, std::map<K, V> & output) {
    StdMapVisitor<K, V> visitor(output);
    deserializer.deserialize_map(visitor);
}

/// @brief Generic `Deserialize` implementation that defers to `de::deserialize<T>()`.
///
/// This Serde implementation uses dynamic dispatch to reduce compilation
/// overhead. If your class does not inherit from `Deserialize`, then you
/// will be required to use this class to use a `Deserializer`.
///
/// `de::accessor(item)` is a helper function that avoids having to write 
/// the template type.
///
/// @tparam T Type of object to deserialize
template <class K, class V>
class Accessor<std::map<K, V>> : public de::Deserialize {
public:
    /// @brief de::Accessor Constructor
    /// @param output Reference of variable to deserialize into
    explicit Accessor(std::map<K, V> & output) : output(output) {
    }

    /// @brief Invoke `de::deserialize<T>()`
    /// @param deserializer Deserializer to extract from
    void deserialize(Deserializer & deserializer) override {
        de::deserialize(deserializer, output);
    }

    /// @brief Get the traits of type T
    /// @return `TypeTraits::of<T>()`
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<std::map<K, V>>();
    };

private:
    /// @brief Reference of variable to deserialize into
    std::map<K, V> & output;
};

/// @brief Helper function to construct `de::Accessor<T>`
/// Avoids having to write the template type during construction.
/// @tparam T Type of object to deserialize
/// @param output Reference of variable to deserialize into
/// @return `de::Accessor<T>`
template <class K, class V>
Accessor<std::map<K, V>> accessor(std::map<K, V> & output) {
    return Accessor<std::map<K, V>>(output);
}

}  // namespace de
}  // namespace kingw
