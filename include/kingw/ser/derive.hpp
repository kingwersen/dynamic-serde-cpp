#pragma once

#include "kingw/serde/derive_common.hpp"
#include "kingw/ser/serializer.hpp"


#define KINGW_SERDE_SERIALIZE_FIELD(name, fn) state.serialize_field(#name, kingw::ser::accessor(fn));

#define DERIVE_SERIALIZE(StructName, ...) \
template <> \
void kingw::ser::serialize<StructName>(kingw::ser::Serializer & serializer, const StructName & object) { \
    auto state = serializer.serialize_struct(); \
    KINGW_SERDE_PP_MAP(KINGW_SERDE_SERIALIZE_FIELD, __VA_ARGS__) \
    state.end(); \
}
