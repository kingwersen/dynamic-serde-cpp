#pragma once

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace de {

// Forward-declare from "kingw/de/deserializer.hpp"
class Deserializer;

template <class T>
void deserialize(Deserializer & deserializer, T & data);

class Deserialize
{
public:
    virtual ~Deserialize() = default;
    virtual void deserialize(Deserializer & Deserializer) = 0;
    virtual TypeTraits traits() const = 0;
};

}  // namespace de
}  // namespace kingw
