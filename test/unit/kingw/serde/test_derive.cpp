#include <gmock/gmock.h>

#include "kingw/mock/ser/mock_serializer.hpp"
#include "kingw/mock/de/mock_deserializer.hpp"
#include "kingw/serde/derive.hpp"

using namespace kingw;
using namespace testing;


namespace {

struct ExampleStruct {
    int a;
    double b;
};

/// FieldDefinition represents a reference to a member variable of a struct.
///
TEST(KingwSerde, DeriveFieldDefinitionConstruction) {
    auto a_field = serde::FieldDefinition<ExampleStruct, int>("a", &ExampleStruct::a);
    EXPECT_EQ(a_field.name, "a");
    EXPECT_EQ(a_field.ptr, &ExampleStruct::a);

    auto b_field = serde::FieldDefinition<ExampleStruct, double>("B", &ExampleStruct::b);
    EXPECT_EQ(b_field.name, "B");
    EXPECT_EQ(b_field.ptr, &ExampleStruct::b);
}

/// FieldDefinition::get(object) provides a reference (const or non-const) to
/// the corresponding field in object.
TEST(KingwSerde, DeriveFieldDefinitionGet) {
    auto field = serde::FieldDefinition<ExampleStruct, int>("a", &ExampleStruct::a);
    
    ExampleStruct example{ 5, 10.0 };
    ASSERT_EQ(example.a, 5);
    field.get(example) = 2;
    EXPECT_EQ(example.a, 2);
    EXPECT_EQ(field.get(example), 2);

    const ExampleStruct example_const{ 5, 10.0 };
    EXPECT_EQ(field.get(example_const), 5);
}

/// A StructDefinition with no fields has field_number == 0 and a struct name.
///
TEST(KingwSerde, DeriveStructDefinitionNoFields) {
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct");
    EXPECT_EQ(defn.struct_name(), serde::string_view("ExampleStruct"));
    EXPECT_EQ(static_cast<std::size_t>(defn.field_number), 0);
}

/// A StructDefinition with fields has field_number = (# of fields) and a struct name.
///
TEST(KingwSerde, DeriveStructDefinitionWithFields) {
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);
    EXPECT_EQ(defn.struct_name(), serde::string_view("ExampleStruct"));
    EXPECT_EQ(static_cast<std::size_t>(defn.field_number), 2);
}

/// StructDefinition::field_names() copies the names of each of the fields in
/// the definition into an array (output parameter).
TEST(KingwSerde, DeriveStructDefinitionFieldNames) {
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    serde::string_view arr[2];
    defn.field_names(arr);
    EXPECT_EQ(arr[0], serde::string_view("a"));
    EXPECT_EQ(arr[1], serde::string_view("b"));
}

/// StructDefinition::inded_of_field_name() returns the field_number of
/// the field whose name matches the input. If no field matches the input,
/// then 0 is returned.
TEST(KingwSerde, DeriveStructDefinitionIndexOfFieldName) {
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    EXPECT_EQ(defn.index_of_field_name("a"), 1);
    EXPECT_EQ(defn.index_of_field_name("b"), 2);
    EXPECT_EQ(defn.index_of_field_name("c"), 0);
    EXPECT_EQ(defn.index_of_field_name(""), 0);
    EXPECT_EQ(defn.index_of_field_name("ab"), 0);
}

/// StructDefinition::serialize() will correctly serialize all members
/// of the defined struct in the provided Serializer.
TEST(KingwSerde, DeriveStructDefinitionSerializeEmpty) {
    // Expected serialization sequence of events.
    ser::MockSerializer serializer;
    InSequence seq;  // The following EXPECT_CALL()s must occur in this order.
    EXPECT_CALL(serializer, struct_begin(serde::string_view("ExampleStruct"), 0))
        .Times(1);
    EXPECT_CALL(serializer, struct_end())
        .Times(1);


    // Define StructDefinition.
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct");

    // Use StructDefinition to serialize an instance of the struct.
    ExampleStruct example{ 5, 10.0 };
    defn.serialize(serializer, example);
}


/// StructDefinition::serialize() will correctly serialize all members
/// of the defined struct in the provided Serializer.
TEST(KingwSerde, DeriveStructDefinitionSerialize) {
    // Expected serialization sequence of events.
    ser::MockSerializer serializer;
    InSequence seq;  // The following EXPECT_CALL()s must occur in this order.
    EXPECT_CALL(serializer, struct_begin(serde::string_view("ExampleStruct"), 2)).Times(1);
    EXPECT_CALL(serializer, struct_serialize_field(serde::string_view("a"), _)).Times(1)
        .WillOnce([&](serde::string_view, const ser::Serialize & value) {
            ser::serialize(serializer, value);  // Must invoke serialize_i32(value)
        });
    EXPECT_CALL(serializer, serialize_i32(5)).Times(1);
    EXPECT_CALL(serializer, struct_serialize_field(serde::string_view("b"), _)).Times(1)
        .WillOnce([&](serde::string_view, const ser::Serialize & value) {
            ser::serialize(serializer, value);  // Must invoke serialize_f64(value)
        });
    EXPECT_CALL(serializer, serialize_f64(10.0)).Times(1);
    EXPECT_CALL(serializer, struct_end()).Times(1);


    // Define StructDefinition.
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    // Use StructDefinition to serialize an instance of the struct.
    ExampleStruct example{ 5, 10.0 };
    defn.serialize(serializer, example);
}

/// StructDefinition::deserialize() with no fields defers to EmptyStructVisitor.
///
TEST(KingwSerde, DeriveStructDefinitionDeserializeEmpty) {
    de::MockDeserializer deserializer;
    EXPECT_CALL(deserializer, deserialize_struct(serde::string_view("ExampleStruct"), _,
            WhenDynamicCastTo<const serde::StructDefinition<ExampleStruct>::EmptyStructVisitor &>(_)))
        .Times(1)
        .WillOnce([&](serde::string_view, const de::Deserializer::FieldNames & field_names, de::Visitor & visitor) {
            // Verify field names.
            ASSERT_EQ(field_names.end() - field_names.begin(), 0);

            // See DeriveStructVisitor* for deserialize implementation.
        });

    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct");
    ExampleStruct example{ 0, 0.0 };
    defn.deserialize(deserializer, example);
}

/// StructDefinition::deserialize() with fields defers to StructVisitor.
///
TEST(KingwSerde, DeriveStructDefinitionDeserialize) {
    de::MockDeserializer deserializer;
    EXPECT_CALL(deserializer, deserialize_struct(serde::string_view("ExampleStruct"), _,
            WhenDynamicCastTo<const serde::StructDefinition<ExampleStruct, double, int>::StructVisitor &>(_)))
        .Times(1)
        .WillOnce([&](serde::string_view, const de::Deserializer::FieldNames & field_names, de::Visitor & visitor) {
            // Verify field names while we're here.
            ASSERT_EQ(field_names.end() - field_names.begin(), 2);
            EXPECT_EQ(field_names.begin()[0], serde::string_view("a"));
            EXPECT_EQ(field_names.begin()[1], serde::string_view("b"));

            // See DeriveStructVisitor* for deserialize implementation.
        });

    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);
    ExampleStruct example{ 0, 0.0 };
    defn.deserialize(deserializer, example);
}

/// Test various functionality of the EmptyStructVisitor which is used
/// if attempting to deserialize into an empty struct type. 
TEST(KingwSerde, DeriveStructVisitorEmpty) {
    serde::StructDefinition<ExampleStruct>::EmptyStructVisitor visitor;
    EXPECT_EQ(serde::string_view("empty struct"), visitor.expecting());

    // If Deserializer map contains items, then throw.
    de::MockMapAccess map_access;
    EXPECT_CALL(map_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_THROW(visitor.visit_map(map_access), de::DeserializationException);
    EXPECT_CALL(map_access, has_next()).Times(1).WillOnce(Return(false));
    EXPECT_NO_THROW(visitor.visit_map(map_access));

    // If Deserializer seq contains items, then throw.
    de::MockSeqAccess seq_access;
    EXPECT_CALL(seq_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_THROW(visitor.visit_seq(seq_access), de::DeserializationException);
    EXPECT_CALL(seq_access, has_next()).Times(1).WillOnce(Return(false));
    EXPECT_NO_THROW(visitor.visit_seq(seq_access));
}

/// Test construction of a basic StructVisitor with members.
///
TEST(KingwSerde, DeriveStructVisitor) {
    // Define ExampleStruct and related FieldDefinitions
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    ExampleStruct example{ 0, 0.0 };
    serde::StructDefinition<ExampleStruct, double, int>::StructVisitor visitor(example, defn);
    EXPECT_EQ(serde::string_view("ExampleStruct"), visitor.expecting());
}

/// StructVisitor::visit_seq() deserializes all members of a struct
/// from an ordered, unnamed sequence of data using de::Deserializer.
TEST(KingwSerde, DeriveStructVisitorDeserializeSeq) {
    // Expected deserialization sequence of events.
    de::MockDeserializer deserializer;
    de::MockSeqAccess seq_access;
    InSequence seq;  // The following EXPECT_CALL()s must occur in this order.

    //
    // Deserialize example.a
    //
    EXPECT_CALL(seq_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(seq_access, next_element(_)).Times(1)
        .WillOnce([&](de::Deserialize & deserialize){
            // Must call deserialize_i32().
            deserialize.deserialize(deserializer);
        });
    EXPECT_CALL(deserializer, deserialize_i32(_)).Times(1)
        .WillOnce([](de::Visitor & visitor) {
            visitor.visit_i32(5);
        });

    //
    // Deserialize example.b
    //
    EXPECT_CALL(seq_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(seq_access, next_element(_)).Times(1)
        .WillOnce([&](de::Deserialize & deserialize){
            // Must call deserialize_f64().
            deserialize.deserialize(deserializer);
        });
    EXPECT_CALL(deserializer, deserialize_f64(_)).Times(1)
        .WillOnce([](de::Visitor & visitor) {
            visitor.visit_f64(10.0);
        });

    //
    // End of deserialization
    //
    EXPECT_CALL(seq_access, has_next()).Times(1).WillOnce(Return(false));


    // Create StructVisitor and an instance of ExampleStruct.
    ExampleStruct example{ 0, 0.0 };
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);
    serde::StructDefinition<ExampleStruct, double, int>::StructVisitor visitor(example, defn);
    
    visitor.visit_seq(seq_access);
    EXPECT_EQ(example.a, 5);
    EXPECT_EQ(example.b, 10.0);
}

/// StructVisitor::visit_seq() must throw an exception if the number of elements in
/// the Deserializer is greater than the number of elements in the struct.
TEST(KingwSerde, DeriveStructVisitorSeqTooLong) {
    // Define ExampleStruct and related FieldDefinitions
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    ExampleStruct example{ 0, 0.0 };
    serde::StructDefinition<ExampleStruct, double, int>::StructVisitor visitor(example, defn);

    de::MockSeqAccess seq_access;
    EXPECT_CALL(seq_access, has_next())
        .Times(3)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(true));  // Error - struct has 2 members, not 3+
    EXPECT_CALL(seq_access, next_element(_))
        .Times(2);  // Don't bother assigning value for this test
    EXPECT_THROW(visitor.visit_seq(seq_access), de::DeserializationException);
}

/// StructVisitor::visit_seq() must throw an exception if the number of elements in
/// the Deserializer is less than the number of elements in the struct.
TEST(KingwSerde, DeriveStructVisitorSeqTooShort) {
    // Define ExampleStruct and related FieldDefinitions
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    ExampleStruct example{ 0, 0.0 };
    serde::StructDefinition<ExampleStruct, double, int>::StructVisitor visitor(example, defn);

    de::MockSeqAccess seq_access;
    EXPECT_CALL(seq_access, has_next()).Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));  // Error - struct has 2 members, not 1
    EXPECT_CALL(seq_access, next_element(_)).Times(1);  // Don't bother assigning value for this test
    EXPECT_THROW(visitor.visit_seq(seq_access), de::DeserializationException);
}

/// StructVisitor::visit_map() deserializes all members of a struct
/// from a set of key-value pairs of data using de::Deserializer.
TEST(KingwSerde, DeriveStructVisitorDeserializeMap) {
    // Expected deserialization sequence of events.
    de::MockDeserializer deserializer;
    de::MockMapAccess map_access;
    InSequence seq;  // The following EXPECT_CALL()s must occur in this order.

    //
    // Deserialize example.a
    //
    EXPECT_CALL(map_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(map_access, next_key(_)).Times(1)
        .WillOnce([&](de::Deserialize & deserialize){
            // Must call deserialize_string().
            deserialize.deserialize(deserializer);
        });
    EXPECT_CALL(deserializer, deserialize_string(_)).Times(1)
        .WillOnce([](de::Visitor & visitor) {
            visitor.visit_string("a");
        });
    EXPECT_CALL(map_access, next_value(_)).Times(1)
        .WillOnce([&](de::Deserialize & deserialize){
            // Must call deserialize_i32().
            deserialize.deserialize(deserializer);
        });
    EXPECT_CALL(deserializer, deserialize_i32(_)).Times(1)
        .WillOnce([](de::Visitor & visitor) {
            visitor.visit_i32(5);
        });

    //
    // Deserialize example.b
    //
    EXPECT_CALL(map_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(map_access, next_key(_)).Times(1)
        .WillOnce([&](de::Deserialize & deserialize){
            // Must call deserialize_string().
            deserialize.deserialize(deserializer);
        });
    EXPECT_CALL(deserializer, deserialize_string(_)).Times(1)
        .WillOnce([](de::Visitor & visitor) {
            visitor.visit_string("b");
        });
    EXPECT_CALL(map_access, next_value(_)).Times(1)
        .WillOnce([&](de::Deserialize & deserialize){
            // Must call deserialize_f64().
            deserialize.deserialize(deserializer);
        });
    EXPECT_CALL(deserializer, deserialize_f64(_)).Times(1)
        .WillOnce([](de::Visitor & visitor) {
            visitor.visit_f64(10.0);
        });

    //
    // End of deserialization
    //
    EXPECT_CALL(map_access, has_next()).Times(1).WillOnce(Return(false));


    // Create StructVisitor and an instance of ExampleStruct.
    ExampleStruct example{ 0, 0.0 };
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);
    serde::StructDefinition<ExampleStruct, double, int>::StructVisitor visitor(example, defn);

    visitor.visit_map(map_access);
    EXPECT_EQ(example.a, 5);
    EXPECT_EQ(example.b, 10.0);
}

/// StructVisitor::visit_map() must throw an exception if a struct key is not
/// found in the Deserializer.
TEST(KingwSerde, DeriveStructVisitorMapKeyNotFound) {
    // Define ExampleStruct and related FieldDefinitions
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);

    ExampleStruct example{ 0, 0.0 };
    serde::StructDefinition<ExampleStruct, double, int>::StructVisitor visitor(example, defn);

    Sequence order;  // The following EXPECT_CALL()s must occur in order
    de::MockDeserializer deserializer;
    de::MockMapAccess map_access;
    EXPECT_CALL(map_access, has_next()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(map_access, next_key(_)).Times(1).WillOnce([&](de::Deserialize & key) {
        // Must call deserializer.deserialize_string()
        key.deserialize(deserializer);
    });
    EXPECT_CALL(deserializer, deserialize_string(_)).Times(1).WillOnce([&](de::Visitor & visitor) {
        visitor.visit_string("c");  // Error - not a struct member name
    });
    EXPECT_THROW(visitor.visit_map(map_access), de::DeserializationException);
}

/// Test misc functionality of FieldNameAccessor that aren't tested in
/// earlier deserialize tests.
TEST(KingwSerde, DeriveFieldNameAccessor) {
    std::size_t output = 0;
    auto defn = serde::StructDefinition<ExampleStruct>("ExampleStruct")
        ("a", &ExampleStruct::a)
        ("b", &ExampleStruct::b);
    decltype(defn)::FieldNameAccessor accessor(defn, output);

    EXPECT_EQ(serde::string_view("struct field name"), accessor.expecting());
    EXPECT_EQ(serde::TypeTraits::of<std::size_t>(), accessor.traits());
}

}  // namespace
