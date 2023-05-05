#include "structs1.hpp"

#include "kingw/serde/derive.hpp"


// template <>
// void kingw::ser::serialize<MyStruct>(kingw::ser::Serializer & serializer, const MyStruct & object) {
//     auto state = serializer.serialize_struct();
//     state.serialize_field("value1", kingw::ser::accessor(object.value1));
//     state.serialize_field("value2", kingw::ser::accessor(object.value2));
//     state.serialize_field("s", kingw::ser::accessor(object.s));
//     state.end();
// }
DERIVE_SERIALIZE(MyStruct,
    value1, object.value1,
    value2, object.value2,
    s,      object.s);


// enum class MyStructFieldsEnum { value1, value2, s };
// template <>
// void kingw::de::deserialize<MyStructFieldsEnum>(kingw::de::Deserializer & deserializer, Field & object) {
//     using FieldsEnum = MyStructFieldsEnum;
//     struct FieldVisitor : public kingw::de::Visitor {
//         explicit FieldVisitor(Field & object) : object(object) { }
//         const char * expecting() const override { return "'value1' or 'value2' or 's'"; }
//         void visit_string(const std::string & accessor) override {
//             if (accessor == "value1") {
//                 object = Field::value1;
//             } else if (accessor == "value2") {
//                 object = Field::value2;
//             } else if (accessor == "s") {
//                 object = Field::s;
//             } else {
//                 throw kingw::de::DeserializationException("unsupported key");
//             }
//         }
//         Field & object;
//     };
//     FieldVisitor visitor(object);
//     deserializer.deserialize_string(visitor);
// }
// template <>
// void kingw::de::deserialize<MyStruct>(kingw::de::Deserializer & deserializer, MyStruct & object) {
//     using FieldsEnum = MyStructFieldsEnum;
//     struct MyStructVisitor : public kingw::de::Visitor {
//         explicit MyStructVisitor(MyStruct & object) : object(object) { }
//         const char * expecting() const override { return "struct MyStruct"; }
//         void visit_map(kingw::de::Deserializer::MapAccess & accessor) override {
//             auto value1_accessor = kingw::de::accessor(object.value1);
//             auto value2_accessor = kingw::de::accessor(object.value2);
//             auto s_accessor = kingw::de::accessor(object.s);
//             while (accessor.has_next()) {
//                 Field field;
//                 kingw::de::Accessor<Field> field_accessor(field);
//                 accessor.next_key(field_accessor);
//                 switch (field) {
//                 case FieldsEnum::value1: accessor.next_value(value1_accessor); break;
//                 case FieldsEnum::value2: accessor.next_value(value2_accessor); break;
//                 case FieldsEnum::s:      accessor.next_value(s_accessor); break;
//                 default: throw kingw::de::DeserializationException("unsupported key");
//                 }
//             }
//         }
//         MyStruct & object;
//     };
//     MyStructVisitor visitor(object);
//     const auto fields = { "value1", "value2", "s", };
//     deserializer.deserialize_struct("MyStruct", fields, visitor);
// }
DERIVE_DESERIALIZE(MyStruct,
    value1, object.value1,
    value2, object.value2,
    s,      object.s);
