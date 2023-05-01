#pragma once

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace de {

// Forward-declare from "kingw/de/deserializer.hpp"
class Deserializer;

/// @brief Extract data from a Deserializer.
///
/// This template function has no default implementation.
/// You will need to create specialized implementations for your classes.
/// You can do this in any source (.cpp) file:
///
/// ```
/// TODO
/// ```
///
/// There are existing implementations for all integral types,
/// strings, `std::vector`, and `std::map`.
///
/// If there is no implementation for your type, then you will get
/// a linker error when you attempt to compile your executable.
/// See the Rust manual for general guidelines (for Rust):
/// https://serde.rs/impl-deserialize.html
/// https://serde.rs/deserialize-struct.html
///
/// The `Deserializer` contains all the data that has yet to be
/// deserialized. Use nested deserialize() calls to extract it.
/// Refer to your `Deserializer` implementation to find out how
/// to initialize it with data and other details.
/// Often has a `from_string()` function or equivalent.
///
/// @tparam T Type of object to deserialize
/// @param deserializer Deserializer to extract from
/// @param output Output location
template <class T>
void deserialize(Deserializer & deserializer, T & output);

/// @brief "Trait"/Interface for deserializable types.
///
/// Prefer to use `deserialize<T>(Deserializer, T)` instead
/// by providing an implementation for your type in any source file.
///
/// `Accessor<T>` is a generic implementation of `Deserialize`
/// that defers to `deserialize<T>(Deserializer, T)`.
class Deserialize
{
public:
    /// @brief Deserialize Destructor.
    virtual ~Deserialize() = default;

    /// @brief Extract this object from the Deserializer. 
    /// @param deserializer Deserializer to extract from
    virtual void deserialize(Deserializer & deserializer) = 0;

    /// @brief Get type information for this object.
    /// You can usually `return TypeTraits::of<T>()`.
    /// @return Type information for this object
    virtual serde::TypeTraits traits() const = 0;
};

}  // namespace de
}  // namespace kingw
