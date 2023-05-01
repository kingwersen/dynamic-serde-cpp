#include "structs1.hpp"

#include "kingw/serde/serialize.hpp"
#include "kingw/serde/accessor.hpp"
using namespace kingw;


template <>
void kingw::ser::serialize<MyStruct>(ser::Serializer & serializer, const MyStruct & value) {
    serializer.serialize_struct()
        .with_field(ser::accessor(value.value1), "value1")
        .with_field(ser::accessor(value.value2), "value2")
        .with_field(ser::accessor(value.s), "s");
}
