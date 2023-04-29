#pragma once

#include <type_traits>
#include <string>


struct TypeTraits {
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

    bool is_bool = false;
    bool is_integral = false;
    bool is_floating_point = false;
    bool is_arithmetic = false;
    bool is_string = false;
    bool is_class = false;
};
