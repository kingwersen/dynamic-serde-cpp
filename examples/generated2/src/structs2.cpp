#include "structs2.hpp"

#include "kingw/ser/serializer.hpp"
#include "kingw/de/deserializer.hpp"
using namespace kingw;


template <>
void kingw::ser::serialize<MyStruct2>(ser::Serializer & serializer, const MyStruct2 & value) {
    auto state = serializer.serialize_struct();
    state.serialize_field("contents", ser::accessor(value.contents));
    state.end();
}


namespace {
    enum class Field { contents };
}

template <>
void kingw::de::deserialize<Field>(de::Deserializer & deserializer, Field & output) {
    struct FieldVisitor : public de::Visitor {
        explicit FieldVisitor(Field & output) : output(output) { }
        const char * expecting() const override { return "'contents'"; }
        void visit_string(const std::string & accessor) override {
            if (accessor == "contents") {
                output = Field::contents;
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
void kingw::de::deserialize<MyStruct2>(de::Deserializer & deserializer, MyStruct2 & output) {
    struct MyStruct2Visitor : public de::Visitor {
        explicit MyStruct2Visitor(MyStruct2 & output) : output(output) { }
        const char * expecting() const override { return "struct MyStruct2"; }
        void visit_map(de::Deserializer::MapAccess & accessor) override {
            auto contents_accessor = de::accessor(output.contents);
            while (accessor.has_next()) {
                Field field;
                de::Accessor<Field> field_accessor(field);
                accessor.next_key(field_accessor);

                switch (field) {
                case Field::contents: accessor.next_value(contents_accessor); break;
                default: throw de::DeserializationException("unsupported key");
                }
            }
        }
        MyStruct2 & output;
    };

    MyStruct2Visitor visitor(output);
    const auto fields = { "contents" };
    deserializer.deserialize_struct("MyStruct2", fields, visitor);
}
