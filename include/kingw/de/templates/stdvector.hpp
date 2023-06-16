#pragma once

#include <vector>

#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace de {

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
class Accessor<std::vector<T>> : public de::Deserialize {
public:
    /// @brief de::Accessor Constructor
    /// @param output Reference of variable to deserialize into
    explicit Accessor(std::vector<T> & output) : output(output) {
    }

    /// @brief Invoke `de::deserialize<T>()`
    /// @param deserializer Deserializer to extract from
    void deserialize(Deserializer & deserializer) override {
        de::deserialize(deserializer, output);
    }

    /// @brief Get the traits of type T
    /// @return `TypeTraits::of<T>()`
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<std::vector<T>>();
    };

private:
    /// @brief Reference of variable to deserialize into
    std::vector<T> & output;
};

/// @brief Helper function to construct `de::Accessor<T>`
/// Avoids having to write the template type during construction.
/// @tparam T Type of object to deserialize
/// @param output Reference of variable to deserialize into
/// @return `de::Accessor<T>`
template <class T>
Accessor<std::vector<T>> accessor(std::vector<T> & output) {
    return Accessor<std::vector<T>>(output);
}

}  // namespace de
}  // namespace kingw
