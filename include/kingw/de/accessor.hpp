#pragma once

#include <vector>
#include <map>

#include "kingw/de/deserialize.hpp"
#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace de {

/// @brief `deserialize()` specialization for C-style arrays.
/// @tparam T Array element type
/// @tparam N Compile-time array length
/// @param deserializer Deserializer to extract from
/// @param output Output location
template <class T, std::size_t N>
void deserialize(Deserializer & deserializer, T (&output)[N]);

/// @brief `deserialize()` specialization for std::vector.
///
/// Since `std::vector` is a template, this definition has to be
/// available anywhere a `std::vector` is deserialized.
///
/// @tparam T Vector element type
/// @param deserializer Deserializer to extract from
/// @param output Output location
template <class T>
void deserialize(Deserializer & deserializer, std::vector<T> & output);

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
void deserialize(Deserializer & deserializer, std::map<K, V> & output);

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
template <class T>
class Accessor : public de::Deserialize {
public:
    /// @brief de::Accessor Constructor
    /// @param output Reference of variable to deserialize into
    explicit Accessor(T & output) : output(output) {
    }

    /// @brief Invoke `de::deserialize<T>()`
    /// @param deserializer Deserializer to extract from
    void deserialize(Deserializer & deserializer) override {
        de::deserialize(deserializer, output);
    }

    /// @brief Get the traits of type T
    /// @return `TypeTraits::of<T>()`
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<T>();
    };

private:
    /// @brief Reference of variable to deserialize into
    T & output;
};

/// @brief Helper function to construct `de::Accessor<T>`
/// Avoids having to write the template type during construction.
/// @tparam T Type of object to deserialize
/// @param output Reference of variable to deserialize into
/// @return `de::Accessor<T>`
template <class T>
Accessor<T> accessor(T & output) {
    return Accessor<T>(output);
}

template <class T, std::size_t N>
void deserialize(Deserializer & deserializer, T (&output)[N]) {
    // TODO
    throw std::logic_error("deserialize() fpr C arrays not yet implemented");
}

/// @brief Generic Visitor class for `std::vector<T>` deserialization
///
/// Used by `deserialize<std::vector<T>>()`.
/// You probably won't manually use this yourself, but you can if you need to.
///
/// @tparam T Vector element type
template <class T>
class StdVectorVisitor : public Visitor {
public:
    /// @brief StdVectorVisitor Constructor
    /// @param output Reference of variable to deserialize into
    explicit StdVectorVisitor(std::vector<T> & output) : output(output) { }

    /// @brief Explanation of what this Visitor is expecting
    /// @return A string literal
    const char* expecting() const override { return "a sequence of items"; }
    
    /// @brief Extract items from `seq` and put them into `output`
    ///
    /// `Deserializer` does not know what type of vector we are.
    ///
    /// First, by calling `Deserializer::deserialize_seq()` we tell the
    /// `Deserializer` that the next item it will deserialize should
    /// be some sort of list/sequence. `seq` is a generic, untyped
    /// representation of the list that it found.
    ///
    /// It is our job in this function to figure out what it found
    /// and convert it into a list of type T.
    ///
    /// We do this by providing an `Accessor<T>` (containing a reference
    /// to the next element in `std::vector<T>`) back to the `Deserializer`
    /// through `seq.next_element()`. The `Deserializer` can
    /// then call `accessor.deserialize()` to fill the data.
    ///
    /// @param seq Data from `Deserializer`
    void visit_seq(Deserializer::SeqAccess & seq) override {
        while (seq.has_next()) {
            output.push_back({});
            de::Accessor<T> accessor(output.back());
            seq.next_element(accessor);
        }
    }

private:
    /// @brief Reference of variable to deserialize into
    std::vector<T> & output;
};

/// @brief `deserialize()` specialization for std::vector.
///
/// Since `std::vector` is a template, this definition has to be
/// available anywhere a `std::vector` is deserialized.
///
/// @tparam T Vector element type
/// @param deserializer Deserializer to extract from
/// @param output Output location
template <class T>
void deserialize(Deserializer & deserializer, std::vector<T> & output) {
    StdVectorVisitor<T> visitor(output);
    deserializer.deserialize_seq(visitor);
}

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

}  // namespace de
}  // namespace kingw
