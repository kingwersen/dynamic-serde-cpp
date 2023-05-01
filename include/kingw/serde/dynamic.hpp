#pragma once

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace ser {

class Serializer;
class Deserializer;

class Dynamic
{
public:
    virtual ~Dynamic() = default;
    virtual void serialize(Serializer & serializer) const = 0;
    virtual void deserialize(Deserializer & deserializer) = 0;
    virtual TypeTraits traits() const = 0;
};

}  // namespace ser
}  // namespace kingw
