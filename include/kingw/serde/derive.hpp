#pragma once

#include <cstring>

#include "kingw/ser/serializer.hpp"
#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace serde {

/// @brief Alias for pointers to fields of structs
///
/// In this example, `value` becomes a reference to `example.foo`:
///   struct Example { int foo = 0; };
///   MemberPtr<Example, int> ptr = &Example::foo;
///   Example example;
///   int & value = example.*ptr;
///
/// @tparam Struct Struct/Class type
/// @tparam Field Member variable type
template <class Struct, class Field>
using MemberPtr = Field Struct::*;


/// @brief Description of a member variable of a class/struct
/// @tparam Struct Type of class/struct
/// @tparam Field Type of member variable
template <class Struct, class Field>
struct FieldDefinition {
    /// @brief Name/identifier of the field
    serde::string_view name;

    /// @brief Pointer to the Field object, given a Struct object
    MemberPtr<Struct, Field> ptr;

    /// @brief FieldDefinition Constructor
    /// @param name Name/identifier of the field
    /// @param ptr Pointer to the Field object, given a Struct object. @see MemberPtr<>
    constexpr FieldDefinition(serde::string_view name, MemberPtr<Struct, Field> ptr)
        : name(name), ptr(ptr) {}

    /// @brief Helper function to get the field described by this class
    /// @param object Instance of Struct to use
    /// @return Reference to Struct's field
    Field & get(Struct & object) const {
        return object.*ptr;
    }

    /// @brief Helper function to get the field described by this class (const)
    /// @param object Instance of Struct to use
    /// @return Reference to Struct's field (const)
    const Field & get(const Struct & object) const {
        return object.*ptr;
    }

    /// @brief Helper function to serialize this field from an object
    /// @param state ser::Serializer helper class, from serialize_struct()
    /// @param input Instance of Struct to serialize
    void serialize(ser::Serializer::SerializeStruct & state, const Struct & input) const {
        ser::Accessor<Field> accessor(get(input));
        state.serialize_field(name, accessor);
    }

    /// @brief Helper function to deserialize this field into an object
    ///
    /// The de::Deserializer implementation may choose to deserialize a structure
    /// as a set of key-value pairs (such as for JSON) or as a sequence of unnamed,
    /// ordered elements (such as binary). This implementation is used for key-value pairs.
    ///
    /// @param map de::Deserializer helper class, from deserialize_struct()
    /// @param output Instance of Struct to deserialize
    void deserialize_map(de::Deserializer::MapAccess & map, Struct & output) const {
        de::Accessor<Field> accessor(get(output));
        map.next_value(accessor);
    }

    /// @brief Helper function to deserialize this field into an object
    ///
    /// The de::Deserializer implementation may choose to deserialize a structure
    /// as a set of key-value pairs (such as for JSON) or as a sequence of unnamed,
    /// ordered elements (such as binary). This implementation is used for sequences.
    ///
    /// @param seq de::Deserializer helper class, from deserialize_struct()
    /// @param output Instance of Struct to deserialize
    void deserialize_seq(de::Deserializer::SeqAccess & seq, Struct & output) const {
        de::Accessor<Field> accessor(get(output));
        seq.next_element(accessor);
    }
};


/// @brief Description of a class/struct that has member variables
///
/// This class should be constructed as follows:
///   struct Example { int Foo; double Bar; };
///   auto defn = StructDefinition<Example>("example")
///     .with_field("foo", &Example::Foo)   // type = int
///     .with_field("bar", &Example::Bar);  // type = double
///
/// "defn" in the above snippet is of type StructDefinition<Example, double, int>.
/// Note that the template parameters are listed in reverse order.
///
/// @tparam Struct Type of class/struct
/// @tparam ...Fields List of member variable types
template <class Struct, class ...Fields>
struct StructDefinition;

/// @brief Base case for StructDefinition<>
///
/// Used to begin construction and also to end recursion.
/// Ex: StructDefinition<Example>("example") contains no fields.
///
/// @tparam Struct Type of class/struct
template <class Struct>
struct StructDefinition<Struct> {
    /// @brief Name/identifier of the struct
    ///
    /// This is ONLY defined in the base case. Recursive StructDefinition do not
    /// define the struct name. Any StructDefinition can access the base name
    /// using struct_name().
    serde::string_view name;

    /// @brief Unique index of the last field (one-based index)
    ///
    /// Also serves as a counter of the number of fields in the struct.
    /// For StructDefinition<Example>, field_number is 0 because there are no fields.
    constexpr static std::size_t field_number = 0;

    /// @brief Get the name of this Struct
    ///
    /// The Struct name is only defined in the base StructDefinition case.
    /// This function obtains the name.
    ///
    /// @return Name/identifier of the struct
    constexpr serde::string_view struct_name() const {
        return name;
    }

    /// @brief Base StructDefinition Constructor
    /// @param name Name/identifier of the struct
    constexpr explicit StructDefinition(serde::string_view name)
        : name(name) {}

    /// @brief Construct a new StructDefinition containing the provided field
    /// @tparam NewField Type of member variable
    /// @param field_name Name/identifier of the member variable
    /// @param ptr Pointer to the member variable. @see MemberPtr<>
    /// @return A new StructDefinition with containing the provided field
    template <class NewField>
    constexpr auto with_field(serde::string_view field_name, MemberPtr<Struct, NewField> ptr) const {
        auto new_field = FieldDefinition<Struct, NewField>(field_name, ptr);
        return StructDefinition<Struct, NewField>(new_field, *this);
    }

    /// @brief Alias to StructDefinition<>::with_field()
    ///
    /// Construct a new StructDefinition containing the provided field.
    /// This alias is used by DERIVE_SERDE() for syntactic sugar.
    ///
    /// @tparam NewField Type of member variable
    /// @param field_name Name/identifier of the member variable
    /// @param ptr Pointer to the member variable. @see MemberPtr<>
    /// @return A new StructDefinition with containing the provided field
    template <class NewField>
    constexpr auto operator()(serde::string_view field_name, MemberPtr<Struct, NewField> ptr) const {
        return with_field(field_name, ptr);
    }

    /// @brief Serialize an instance of Struct
    /// @param serializer ser::Serializer to serialize into
    /// @param input Instance of Struct to serialize
    void serialize(ser::Serializer & serializer, const Struct & input) const {
        ser::Serializer::SerializeStruct state = serializer.serialize_struct(struct_name(), field_number);
        serialize_recurse(state, input);
        state.end();
    }

    /// @brief Recursive helper function for serialize() to invoke for each field
    /// @param state ser::Serializer helper class, from serialize_struct()
    /// @param input Instance of Struct to serialize
    void serialize_recurse(ser::Serializer::SerializeStruct & state, const Struct & input) const {
        // No-op. Stop recursing.
    }

    /// @brief Deserialize an instance of Struct
    /// @param deserializer de::Deserialize to deserialize from
    /// @param output Instance of Struct to deserialize
    void deserialize(de::Deserializer & deserializer, Struct & output) const {
        // This struct has no fields.
        // If deserialize returns any fields, then throw an exception.
        EmptyStructVisitor visitor;
        deserializer.deserialize_struct(struct_name(), {}, visitor);
    }

    /// @brief Recursive helper function for deserialize() to invoke for each field
    /// @param map de::Deserializer helper class, from deserialize_struct()
    /// @param field_index Index of the field to apply to
    /// @param output Instance of Struct to deserialize
    void deserialize_map_recurse(de::Deserializer::MapAccess & map, std::size_t field_index, Struct & output) const {
        // This function call is the end of the recursion chain.
        // If we haven't found the matching field by now, then it doesn't exist.
        throw de::DeserializationException("unsupported key");
    }

    /// @brief Recursive helper function for deserialize() to invoke for each field
    /// @param seq de::Deserializer helper class, from deserialize_struct()
    /// @param output Instance of Struct to deserialize
    void deserialize_seq_recurse(de::Deserializer::SeqAccess & seq, Struct & output) const {
        // No-op. Stop recursing.
    }

    /// @brief Helper function that recursively inputs field names into names_arr
    /// @param names_arr Existing array with at least `field_number` elements
    void field_names(serde::string_view* names_arr) const {
        // No-op. Stop recursing.
    }

    /// @brief Obtain the `field_number` of the field named `field_name`, or 0
    /// @param field_name Name/identifier of the field
    /// @return A `field_number` (one-based index), or 0 if the field was not found
    std::size_t index_of_field_name(serde::string_view field_name) const {
        // No-op. Stop recursing.
        // Default value of 0 means that the field name was not found.
        return 0;
    }

    /// @brief Custom visitor that throws an exception if any fields are found
    ///
    /// This StructDefinition has no fields. There's nowhere to put the data.
    struct EmptyStructVisitor : public de::Visitor {
        /// @brief Human-readable hint at what the visitor is expecting
        /// @return A string literal
        const char* expecting() const override {
            return "empty struct";
        }

        /// @brief Interpret and deserialize a map into the output struct instance
        /// @param map de::Deserializer helper object for deserializing key-value pairs
        void visit_map(de::Deserializer::MapAccess & map) override {
            if (map.has_next()) {
                throw de::DeserializationException("expected struct with no fields");
            }
        }

        /// @brief Interpret and deserialize a map into the output struct instance
        /// @param seq de::Deserializer helper object for deserializing a sequence of elements
        void visit_seq(de::Deserializer::SeqAccess & seq) override {
            if (seq.has_next()) {
                throw de::DeserializationException("expected struct with no fields");
            }
        }
    };
};

/// @brief Description of a class/struct that has member variables
///
/// This class should be constructed as follows:
///   struct Example { int Foo; double Bar; };
///   auto defn = StructDefinition<Example>("example")
///     .with_field("foo", &Example::Foo)   // type = int
///     .with_field("bar", &Example::Bar);  // type = double
///
/// "defn" in the above snippet is of type StructDefinition<Example, double, int>.
/// Note that the template parameters are listed in reverse order.
///
/// Example class and corresponding StructDefinition:
///   struct Example {
///       int Field1;
///       int Field2;
///       int Field3;
///   };
///
///   StructDefinition<Example, Field3, Field2, Field1> {
///       StructDefinition<Example, Field2, Field1> {
///           StructDefinition<Example, Field1> {
///               StructDefinition<Example> {
///                   std::size_t field_number = 0;
///               };
///               std::size_t field_number = 1;
///               const char* field.name = "Field1";
///           };
///           std::size_t field_number = 2;
///           const char* field.name = "Field2";
///       };
///       std::size_t field_number = 3;
///       const char* field.name = "Field3";
///   };
///
/// @tparam Struct Type of class/struct
/// @tparam Field Type of member variable
/// @tparam ...PreviousFields List of previously-defined member variable types
template <class Struct, class Field, class ...PreviousFields>
struct StructDefinition<Struct, Field, PreviousFields...> {
    /// @brief Description of one member variable of Struct
    FieldDefinition<Struct, Field> field;

    /// @brief Description of other, recursively-defined member variables of Struct
    StructDefinition<Struct, PreviousFields...> previous_fields;

    /// @brief Unique index of the last field (one-based index)
    ///
    /// Also serves as a counter of the number of fields in the struct.
    constexpr static std::size_t field_number = sizeof...(PreviousFields) + 1;

    /// @brief Get the name of this Struct
    ///
    /// The Struct name is only defined in the base StructDefinition case.
    /// This function obtains the name.
    ///
    /// @return Name/identifier of the struct
    constexpr serde::string_view struct_name() const {
        return previous_fields.struct_name();
    }

    /// @brief Construct a new StructDefinition with containing the provided field
    ///
    /// Users will usually NOT use this constructor. Instead, construct the StructDefinition
    /// class and use with_field() repeatedly. For example:
    ///   struct Example { int Foo; double Bar; };
    ///   auto defn = StructDefinition<Example>("example")
    ///     .with_field("foo", &Example::Foo)
    ///     .with_field("bar", &Example::Bar);
    ///
    /// @see with_field()
    ///
    /// @param field Prebuilt FieldDefinition for the new field
    /// @param previous_fields Existing StructDefinition<> to append to
    constexpr explicit StructDefinition(FieldDefinition<Struct, Field> field, StructDefinition<Struct, PreviousFields...> previous_fields)
        : field(field), previous_fields(previous_fields) {}

    /// @brief Construct a new StructDefinition containing the provided field
    /// @tparam NewField Type of member variable
    /// @param field_name Name/identifier of the member variable
    /// @param ptr Pointer to the member variable. @see MemberPtr<>
    /// @return A new StructDefinition with containing the provided field
    template <class NewField>
    constexpr auto with_field(const char* field_name, MemberPtr<Struct, NewField> ptr) const {
        auto new_field = FieldDefinition<Struct, NewField>(field_name, ptr);
        return StructDefinition<Struct, NewField, Field, PreviousFields...>(new_field, *this);
    }

    /// @brief Alias to StructDefinition<>::with_field()
    ///
    /// Construct a new StructDefinition containing the provided field.
    /// This alias is used by DERIVE_SERDE() for syntactic sugar.
    ///
    /// @tparam NewField Type of member variable
    /// @param field_name Name/identifier of the member variable
    /// @param ptr Pointer to the member variable. @see MemberPtr<>
    /// @return A new StructDefinition with containing the provided field
    template <class NewField>
    constexpr auto operator()(const char* field_name, MemberPtr<Struct, NewField> ptr) const {
        return with_field(field_name, ptr);
    }

    /// @brief Serialize an instance of Struct
    /// @param serializer ser::Serializer to serialize into
    /// @param input Instance of Struct to serialize
    void serialize(ser::Serializer & serializer, const Struct & input) const {
        std::size_t num_fields = field_number;  // field_number is a 1-based index. This is the last field in the struct.
        ser::Serializer::SerializeStruct state = serializer.serialize_struct(struct_name(), num_fields);
        serialize_recurse(state, input);
        state.end();
    }

    /// @brief Recursive helper function for serialize() to invoke for each field
    ///
    /// Iterate over each field, starting with the first, and serialize() from them.
    ///   1. ser::serialize(serializer, field1)
    ///   2. ser::serialize(serializer, field2)
    ///   3. ser::serialize(serializer, field3)
    ///
    /// @param state ser::Serializer helper class, from serialize_struct()
    /// @param input Instance of Struct to serialize
    void serialize_recurse(ser::Serializer::SerializeStruct & state, const Struct & input) const {
        previous_fields.serialize_recurse(state, input);
        field.serialize(state, input);
    }

    /// @brief Deserialize an instance of Struct
    /// @param deserializer de::Deserialize to deserialize from
    /// @param output Instance of Struct to deserialize
    void deserialize(de::Deserializer & deserializer, Struct & output) const {
        // Collect the list of field names. Avoid dynamic memory allocation.
        // TODO: Initializer list? Span?
        serde::string_view names[sizeof...(PreviousFields) + 1] = {};
        field_names(names);
        de::Deserializer::FieldNames field_names(std::begin(names), std::end(names));

        // Deserialize using a custom visitor that will invoke deserialize_recurse().
        StructVisitor visitor(output, *this);
        deserializer.deserialize_struct(struct_name(), field_names, visitor);
    }

    /// @brief Recursive helper function for deserialize() to invoke for each field
    ///
    /// This function call indicates through field_index which field should be
    /// deserialized into. Find it and run deserialize(). This function will usually
    /// be invoked multiple times (ideally once per field in the class/struct).
    ///
    /// Example operations:
    ///   1. field_number(1) == 2 ?:no -> continue
    ///   2. field_number(2) == 2 ?:yes -> de::deserialize(deserializer, field2)
    ///   3. field_number(3) == 2 ?:no -> continue
    ///
    /// The field_index parameter is obtained by deserializing using FieldNameAccessor.
    ///
    /// @param map de::Deserializer helper class, from deserialize_struct()
    /// @param field_index Index of the field to apply to
    /// @param output Instance of Struct to deserialize
    void deserialize_map_recurse(de::Deserializer::MapAccess & map, std::size_t field_index, Struct & output) const {
        // Deserialize using the field that matches `field_index` only.
        if (field_index == field_number) {
            field.deserialize_map(map, output);
        } else {
            previous_fields.deserialize_map_recurse(map, field_index, output);
        }
    }

    /// @brief Recursive helper function for deserialize() to invoke for each field
    ///
    /// Iterate over each field, starting with the first, and deserialize() into them.
    ///   1. de::deserialize(deserializer, field1)
    ///   2. de::deserialize(deserializer, field2)
    ///   3. de::deserialize(deserializer, field3)
    ///
    /// @param seq de::Deserializer helper class, from deserialize_struct()
    /// @param output Instance of Struct to deserialize
    void deserialize_seq_recurse(de::Deserializer::SeqAccess & seq, Struct & output) const {
        // Recurse first - the deepest item is first.
        previous_fields.deserialize_seq_recurse(seq, output);
        if (seq.has_next()) {
            field.deserialize_seq(seq, output);
        } else {
            throw de::DeserializationException("number of elements in sequence does not match struct fields");
        }
    }

    /// @brief Helper function that recursively inputs field names into names_arr
    ///
    /// Example operations:
    ///   1. names_arr[0] = "Field1";
    ///   2. names_arr[1] = "Field2";
    ///   3. names_arr[2] = "Field3";
    ///
    /// @param names_arr Existing array with at least `field_number` elements
    constexpr void field_names(serde::string_view* names_arr) const {
        previous_fields.field_names(names_arr);
        names_arr[field_number-1] = field.name;
    }

    /// @brief Obtain the `field_number` of the field named `field_name`, or 0
    ///
    /// The recursion base case in StructDefinition<Struct> returns 0.
    /// While the return value is still 0, compare the input field name to the name
    /// of the current field. If it matches, then return the current field number
    /// instead of 0. Else, continue to return 0.
    ///
    /// If we run out of fields and the value is still 0, then the field name was not found.
    ///
    /// Example operations (field_name = "Field2"):
    ///   1. result = 0
    ///   2. result == 0 ?:yes -> field_name == "Field1" ?:no -> continue
    ///   3. result == 0 ?:yes -> field_name == "Field2" ?:yes -> result = 2
    ///   4. result == 0 ?:no -> continue
    ///
    /// @param field_name Name/identifier of the field
    /// @return A `field_number` (one-based index), or 0 if the field was not found
    constexpr std::size_t index_of_field_name(serde::string_view field_name) const {
        std::size_t result = previous_fields.index_of_field_name(field_name);
        if (result == 0 && field_name == field.name) {
            result = field_number;
        }
        return result;
    }

    /// @brief Custom visitor for deserializing struct fields
    struct StructVisitor : public de::Visitor {
        /// @brief Struct instance to deserialize into
        Struct & output;

        /// @brief Description of Struct
        StructDefinition defn;

        /// @brief StructVisitor Constructor
        /// @param output Struct instance to deserialize into
        /// @param defn Description of Struct
        explicit StructVisitor(Struct & output, StructDefinition defn)
            : output(output), defn(defn) {}

        /// @brief Human-readable hint at what the visitor is expecting
        /// @return A string literal
        const char* expecting() const override {
            return defn.struct_name().data();
        }

        /// @brief Interpret and deserialize a map into the output struct instance
        /// @param map de::Deserializer helper object for deserializing key-value pairs
        void visit_map(de::Deserializer::MapAccess & map) override {
            // For each key-value-pair, deserialize value into the field with
            // the matching key (field name).
            std::size_t field_index = 0;
            FieldNameAccessor visitor(defn, field_index);
            while (map.has_next()) {
                map.next_key(visitor);  // Put key to field_index, or 0
                defn.deserialize_map_recurse(map, field_index, output);
            }
        }

        /// @brief Interpret and deserialize a sequence into the output struct instance
        /// @param seq de::Deserializer helper object for deserializing a sequence of elements
        void visit_seq(de::Deserializer::SeqAccess & seq) override {
            // Deserialize each field in order. Must match exactly.
            defn.deserialize_seq_recurse(seq, output);
            if (seq.has_next()) {
                throw de::DeserializationException("number of elements in sequence does not match struct fields");
            }
        }
    };

    /// @brief Helper class to deserialize a field name into a `field_number` or 0
    ///
    /// Usage:
    ///   std::size_t output = 0;  // Don't need to reinitialize every time
    ///   FieldNameAccessor accessor(defn, output);
    ///   de::deserialize(deserializer, accessor);
    ///   // output is now a valid number
    struct FieldNameAccessor : public de::Deserialize, public de::Visitor {
        /// @brief Description of a class/struct that has member variables
        StructDefinition defn;

        /// @brief Output param. One-based index - a `field_number` or 0
        std::size_t & output;

        /// @brief FieldNameAccessor Constructor
        /// @param defn Description of a class/struct that has member variables
        /// @param output Output param. One-based index - a `field_number` or 0
        FieldNameAccessor(StructDefinition defn, std::size_t & output)
            : defn(defn), output(output) {}

        /// @brief Extract this object from the Deserializer
        /// @param deserializer Deserializer to extract from
        void deserialize(de::Deserializer & deserializer) override {
            // Get the field name. Should invoke `visit_string()` of
            // this object, see definitions below.
            deserializer.deserialize_string(*this);
        }

        /// @brief Get type information for this object (std::size_t)
        /// @return Type information for this object
        serde::TypeTraits traits() const override {
            return serde::TypeTraits::of<std::size_t>();
        }

        /// @brief Human-readable hint at what the visitor is expecting
        /// @return A string literal
        const char* expecting() const override {
            // TODO: More precise? Would be cool to list the possible options.
            // But without dynamically allocating memory? I dunno.
            return "struct field name";
        }

        /// @brief Set `output` to the `field_number` of the field named `key`, or 0
        /// @param key Name/identifier of the field
        void visit_string(serde::string_view value) override {
            output = defn.index_of_field_name(value);
        }
    };
};

/// @brief ser::serialize<T>() implementation helper
/// @see DERIVE_SERDE() comments for usage info
#define DERIVE_SERIALIZE(StructName, ...) \
template <> \
void kingw::ser::serialize<StructName>(kingw::ser::Serializer & serializer, const StructName & input) { \
    using Self = StructName; \
    auto defn = kingw::serde::StructDefinition<StructName>(#StructName) __VA_ARGS__ ; \
    defn.serialize(serializer, input); \
}

/// @brief de::deserialize<T>() implementation helper
/// @see DERIVE_SERDE() comments for usage info
#define DERIVE_DESERIALIZE(StructName, ...) \
template <> \
void kingw::de::deserialize<StructName>(kingw::de::Deserializer & deserializer, StructName & output) { \
    using Self = StructName; \
    auto defn = kingw::serde::StructDefinition<StructName>(#StructName) __VA_ARGS__ ; \
    defn.deserialize(deserializer, output); \
}

/// @brief ser::serialize<T>() and de::deserialize<T>() implementation helper
///
/// Example usage:
///   struct Example { int foo; double bar; };
///   DERIVE_SERDE(Example,
///     ("Foo", &Self::foo)
///     ("Bar", &Self::bar));
///
/// This macro is used to define ser::serialize<T>() and de::deserialize<T>(),
/// which are the core of the "Data Model" half of the Serde framework.
/// For each type T, these two functions declare all member variables that should
/// be serialized and deserialized and how each operation should be performed.
/// Then, the "Data Format" half of the Serde framework uses these definitions to
/// format your data into and out of serialized form, such as JSON or XML.
///
/// The first argument of this macro is the name of the struct/class.
/// The remaining arguments should be a list of the member variables of the
/// struct/class thata should be serialized and deserialized, in the following
/// format (NOT comma separated):
///   ("identifier1", &Self::field1)
///   ("identifier2", &Self::field2)
///
/// For example, the above may serialize into the following JSON string:
///   { "identifier1": 5, "identifier2": 10.0 }
///
/// `Self` is an alias for the class/struct name, for convenience.
/// "identifier" can be any string literal, but cannot be repeated in the same class.
/// You do not need to specify the type of each member variable. As long as each
/// member's type also has ser::serialize<T>()/de::deserialize<T>() defined,
/// then those definitions will automatically be inferred and invoked.
/// If a member type does NOT have Serde defined, then the C++ build will
/// fail with a linker error indicating that the function specification
/// was not found. Serde definitions may be defined in different source files
/// or libraries and linked together. It is recommended to DERIVE_SERDE() in
/// source files and NOT in header files or you may impact your compilation speed.
///
/// You may use DERIVE_SERIALIZE() and DERIVE_DESERIALIZE() instead if you only
/// wish to have one of the two defined. DERIVE_SERDE() does both.
///
/// If you have a more complicated implementation, then you may need to
/// manually implement Serde for your type. I recommend keeping your I/O types
/// as simple as possible though... See Rust's guides here:
/// https://serde.rs/impl-serialize.html
/// https://serde.rs/impl-deserialize.html
/// https://serde.rs/deserialize-struct.html
///
/// DERIVE_SERDE() attributes, etc., are WIP. Sorry.
///
/// This DERIVE_SERDE() impl is almost entirely based on injae/serdepp's BRILLIANT design:
/// https://github.com/injae/serdepp/blob/main/include/serdepp/serializer.hpp
///
/// @param StructName Type of class/struct
/// @param ... List of ("identifier", &Self::member)
#define DERIVE_SERDE(StructName, ...) \
DERIVE_SERIALIZE(StructName, __VA_ARGS__); \
DERIVE_DESERIALIZE(StructName, __VA_ARGS__);

}  // namespace serde
}  // namespace kingw
