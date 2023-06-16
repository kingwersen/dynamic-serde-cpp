#pragma once

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace ser {

// Forward-declare from "kingw/ser/serializer.hpp"
class Serializer;

/// @brief Insert data into a Serializer.
///
/// This template function has no default implementation.
/// You will need to create specialized implementations for your classes.
/// You can do this in any source (.cpp) file:
///
/// ```
/// #include <kingw/ser/serializer.hpp>
/// using namespace kingw;
///
/// template <>
/// void ser::serialize<Example>(ser::Serializer & serializer, const Example & data) {
///     serializer.serialize_struct()
///         .with_field(ser::accessor(data.foo), "foo")
///         .with_field(ser::accessor(data.bar), "bar");   
/// }
/// ```
///
/// There are existing implementations for all integral types,
/// strings, `std::vector`, and `std::map`.
///
/// If there is no implementation for your type, then you will get
/// a linker error when you attempt to compile your executable.
/// See the Rust manual for general guidelines (for Rust):
/// https://serde.rs/impl-serialize.html
///
/// The `Serializer` contains storage of all items serialized
/// into it in a stateful manner that allows for nested serialization.
/// Refer to your `Serializer` implementation to find out how
/// to get the serialized data out of it and other details.
/// Often has a `to_string()` function or equivalent.
///
/// @tparam T Type of object to serialize
/// @param serializer Serializer to insert into
/// @param data Item to serialize
template <class T>
void serialize(ser::Serializer & serializer, const T & data);

/// @brief "Trait"/Interface for serializable types.
///
/// Prefer to use `serialize<T>(Serializer, T)` instead
/// by providing an implementation for your type in any source file.
///
/// `Accessor<T>` is a generic implementation of `Serialize`
/// that defers to `serialize<T>(Serializer, T)`.
class Serialize
{
public:
    /// @brief Serialize Destructor.
    virtual ~Serialize() = default;

    /// @brief Insert this object into the Serializer. 
    /// @param serializer Serializer to insert into
    virtual void serialize(ser::Serializer & serializer) const = 0;

    /// @brief Get type information for this object.
    /// You can usually `return TypeTraits::of<T>()`.
    /// @return Type information for this object
    virtual serde::TypeTraits traits() const = 0;
};

}  // namespace ser
}  // namespace kingw
