#pragma once

#include "kingw/serde/derive_common.hpp"
#include "kingw/ser/serializer.hpp"


#define KINGW_SERDE_SERIALIZE_FIELD(name, fn) state.serialize_field(#name, kingw::ser::accessor(fn));

// TODO: Replace this hack to get the number of fields.
// It increments by 1 for every field in __VA_ARGS__.
#define KINGW_SERDE_NUM_FIELDS(name, fn) +1


#define DERIVE_SERIALIZE(StructName, ...) \
template <> \
void kingw::ser::serialize<StructName>(kingw::ser::Serializer & serializer, const StructName & object) { \
    auto state = serializer.serialize_struct(#StructName, 0 KINGW_SERDE_PP_MAP(KINGW_SERDE_NUM_FIELDS, __VA_ARGS__)); \
    KINGW_SERDE_PP_MAP(KINGW_SERDE_SERIALIZE_FIELD, __VA_ARGS__) \
    state.end(); \
}
