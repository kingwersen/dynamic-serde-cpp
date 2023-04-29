#include "generated/structs.hpp"

#include "kingw/serde/serialize.hpp"

#include <iostream>


using namespace kingw;

template <>
void kingw::ser::serialize<MyStruct2>(ser::Serializer & serializer, const MyStruct2 & value) {
    auto structs = serializer.serialize_struct();
    structs.serialize_field(ser::accessor(value.contents), "contents");
}

template <>
void kingw::ser::serialize<MyStruct>(ser::Serializer & serializer, const MyStruct & value) {
    auto structs = serializer.serialize_struct();
    structs.serialize_field(ser::accessor(value.value1), "value1");
    structs.serialize_field(ser::accessor(value.value2), "value2");
    structs.serialize_field(ser::accessor(value.s), "s");
}

void foo() {}  // TODO: Some weird link problem requires this.
