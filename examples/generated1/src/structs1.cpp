#include "structs1.hpp"

#include "kingw/ser/serializer.hpp"
#include "kingw/de/deserializer.hpp"
using namespace kingw;


template <>
void kingw::ser::serialize<MyStruct>(ser::Serializer & serializer, const MyStruct & value) {
    auto state = serializer.serialize_struct();
    state.serialize_field("value1", ser::accessor(value.value1));
    state.serialize_field("value2", ser::accessor(value.value2));
    state.serialize_field("s", ser::accessor(value.s));
    state.end();
}


namespace {
    enum class Field { value1, value2, s };
}

template <>
void kingw::de::deserialize<Field>(de::Deserializer & deserializer, Field & output) {
    struct FieldVisitor : public de::Visitor {
        explicit FieldVisitor(Field & output) : output(output) { }
        const char * expecting() const override { return "'value1' or 'value2' or 's'"; }
        void visit_string(const std::string & accessor) override {
            if (accessor == "value1") {
                output = Field::value1;
            } else if (accessor == "value2") {
                output = Field::value2;
            } else if (accessor == "s") {
                output = Field::s;
            } else {
                throw de::DeserializationException("unsupported key");
            }
        }
        Field & output;
    };
    
    FieldVisitor visitor(output);
    deserializer.deserialize_string(visitor);
}

template <>
void kingw::de::deserialize<MyStruct>(de::Deserializer & deserializer, MyStruct & output) {
    struct MyStructVisitor : public de::Visitor {
        explicit MyStructVisitor(MyStruct & output) : output(output) { }
        const char * expecting() const override { return "struct MyStruct"; }
        void visit_map(de::Deserializer::MapAccess & accessor) override {
            auto value1_accessor = de::accessor(output.value1);
            auto value2_accessor = de::accessor(output.value2);
            auto s_accessor = de::accessor(output.s);
            while (accessor.has_next()) {
                Field field;
                de::Accessor<Field> field_accessor(field);
                accessor.next_key(field_accessor);

                switch (field) {
                case Field::value1: accessor.next_value(value1_accessor); break;
                case Field::value2: accessor.next_value(value2_accessor); break;
                case Field::s:      accessor.next_value(s_accessor); break;
                default: throw de::DeserializationException("unsupported key");
                }
            }
        }
        MyStruct & output;
    };

    MyStructVisitor visitor(output);
    const auto fields = { "value1", "value2", "s" };
    deserializer.deserialize_struct("MyStruct", fields, visitor);
}
