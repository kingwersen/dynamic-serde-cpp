#include "structs1.hpp"

#include "kingw/ser/serialize.hpp"
#include "kingw/ser/accessor.hpp"
using namespace kingw;


template <>
void kingw::ser::serialize<MyStruct>(ser::Serializer & serializer, const MyStruct & value) {
    auto state = serializer.serialize_struct();
    state.serialize_field("value1", ser::accessor(value.value1));
    state.serialize_field("value2", ser::accessor(value.value2));
    state.serialize_field("s", ser::accessor(value.s));
    state.end();
}
