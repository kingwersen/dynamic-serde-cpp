#pragma once

#include <gmock/gmock.h>

#include <kingw/ser/serialize.hpp>


namespace kingw {
namespace ser {

class MockSerialize : public Serialize {
public:
    MOCK_METHOD(void, serialize, (Serializer &), (const override));
    MOCK_METHOD(kingw::serde::TypeTraits, traits, (), (const override));
};

}  // namespace ser
}  // namespace kingw
