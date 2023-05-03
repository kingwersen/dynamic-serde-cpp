#include "structs2.hpp"

#include "kingw/ser/serialize.hpp"
#include "kingw/ser/accessor.hpp"
using namespace kingw;


template <>
void kingw::ser::serialize<MyStruct2>(ser::Serializer & serializer, const MyStruct2 & value) {
    auto state = serializer.serialize_struct();
    state.serialize_field("contents", ser::accessor(value.contents));
    state.end();
}
