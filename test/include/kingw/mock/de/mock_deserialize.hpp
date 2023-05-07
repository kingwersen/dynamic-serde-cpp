#pragma once

#include <gmock/gmock.h>

#include <kingw/de/deserialize.hpp>


namespace kingw {
namespace de {

class MockDeserialize : public Deserialize {
public:
    MOCK_METHOD(void, deserialize, (Deserializer &), (override));
    MOCK_METHOD(kingw::serde::TypeTraits, traits, (), (const override));
};

}  // namespace de
}  // namespace kingw

