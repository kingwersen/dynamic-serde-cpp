#pragma once

#include "kingw/serde/type_traits.hpp"


namespace kingw {
namespace ser {

// Defined in "kingw/serde/serialize.hpp"
class Serializer;

class Dynamic
{
public:
    virtual ~Dynamic() = default;
    virtual void serialize(Serializer & serializer) const = 0;
    virtual TypeTraits traits() const = 0;
};

}  // namespace ser
}  // namespace kingw
