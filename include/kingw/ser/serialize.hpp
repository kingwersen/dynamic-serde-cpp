#pragma once

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace ser {

// Forward-declare from "kingw/ser/serializer.hpp"
class Serializer;

template <class T>
void serialize(Serializer & serializer, const T & data);

class Serialize
{
public:
    virtual ~Serialize() = default;
    virtual void serialize(Serializer & serializer) const = 0;
    virtual TypeTraits traits() const = 0;
};

}  // namespace ser
}  // namespace kingw
