#pragma once

#include <type_traits>
#include <string>


namespace kingw {
namespace serde {

/// @brief Common type information, available dynamically.
///
/// Required by `ser::Serialize` and `de::Deserialize` so `ser::Serializer`
/// and `de::Deserializer` can dynamically learn useful information about
/// the type it is working with.
///
/// For example, in JSON, a map key must be a string.
/// A JSON serializer can use `accessor.traits().is_string` to verify.
///
/// You can use `return TypeTraits::of<T>()` most of the time.
struct TypeTraits {
    /// @brief Compile-time expression to get traits for a type
    /// @tparam T Type to get traits for
    /// @return Traits of type T
    template <class T>
    constexpr static TypeTraits of() {
        TypeTraits traits;
        traits.is_bool              = std::is_same<T, bool>::value;
        traits.is_integral          = std::is_integral<T>::value;
        traits.is_floating_point    = std::is_floating_point<T>::value;
        traits.is_arithmetic        = std::is_arithmetic<T>::value;
        traits.is_string            = std::is_same<T, std::string>::value;
        traits.is_class             = std::is_class<T>::value;
        return traits;
    }

    /// @brief Various traits
    /// See <type_traits>: https://en.cppreference.com/w/cpp/header/type_traits
    bool is_bool = false;
    bool is_integral = false;
    bool is_floating_point = false;
    bool is_arithmetic = false;
    bool is_string = false;
    bool is_class = false;
};

}  // namespace serde
}  // namespace kingw
