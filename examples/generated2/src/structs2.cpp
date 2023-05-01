#include "structs2.hpp"

#include "kingw/serde/serialize.hpp"
#include "kingw/serde/accessor.hpp"
using namespace kingw;


template <>
void kingw::ser::serialize<MyStruct2>(ser::Serializer & serializer, const MyStruct2 & value) {
    serializer.serialize_struct()
        .with_field(ser::accessor(value.contents), "contents");
}
