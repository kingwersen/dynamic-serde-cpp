#pragma once

#include <cstring>

#include "kingw/ser/serializer.hpp"
#include "kingw/de/deserializer.hpp"


template <class Struct, class Field>
using MemberPtr = Field Struct::*;


template <class Struct, class Field>
struct FieldDefinition {
    const char* name;
    MemberPtr<Struct, Field> ptr;

    FieldDefinition(const char* name, MemberPtr<Struct, Field> ptr)
        : name(name), ptr(ptr) {}

    Field & get(Struct & object) const {
        return object.*ptr;
    }

    const Field & get(const Struct & object) const {
        return object.*ptr;
    }

    void serialize(kingw::ser::Serializer::SerializeStruct & state, const Struct & object) const {
        state.serialize_field(name, kingw::ser::accessor(get(object)));
    }

    void deserialize(kingw::de::Deserializer::MapAccess & map, Struct & object) const {
        kingw::de::Accessor<Field> accessor(get(object));
        map.next_value(accessor);
    }
};


template <class Struct, class ...Fields>
struct StructDefinition;

template <class Struct>
struct StructDefinition<Struct> {
    const char* name;
    constexpr static std::size_t field_number = 0;

    const char* struct_name() const {
        return name;
    }

    explicit StructDefinition(const char* name)
        : name(name) {}

    template <class NewField>
    constexpr auto with_field(const char* field_name, MemberPtr<Struct, NewField> ptr) const {
        auto new_field = FieldDefinition<Struct, NewField>(field_name, ptr);
        return StructDefinition<Struct, NewField>(new_field, *this);
    }

    template <class NewField>
    constexpr auto operator()(const char* field_name, MemberPtr<Struct, NewField> ptr) const {
        return with_field(field_name, ptr);
    }

    void serialize(kingw::ser::Serializer & serializer, const Struct & input) const {
        kingw::ser::Serializer::SerializeStruct state = serializer.serialize_struct(struct_name(), field_number);
        serialize_recurse(state, input);
        state.end();
    }

    void serialize_recurse(kingw::ser::Serializer::SerializeStruct & state, const Struct & input) const {
        // No-op. Stop recursing.
    }

    struct Visitor : public kingw::de::Visitor {
        const char* expecting() const override { return "empty struct"; }
        void visit_map(kingw::de::Deserializer::MapAccess & accessor) override {
            if (accessor.has_next()) {
                throw kingw::de::DeserializationException("struct has no fields");
            }
        }
    };

    void deserialize(kingw::de::Deserializer & deserializer, Struct & output) const {
        Visitor visitor;
        deserializer.deserialize_struct(struct_name(), {}, visitor);
    }

    void deserialize_field(kingw::de::Deserializer::MapAccess & accessor, std::size_t field_index, Struct & output) const {
        throw kingw::de::DeserializationException("unsupported key");
    }

    std::size_t index_of_field_name(const char* field_name, std::size_t len) const {
        // No-op. Stop recursing.
        // Default value of 0 means that the field name was not found.
        return 0;
    }

    void field_names(const char** names_arr) const {
        // No-op. Stop recursing.
    }
};

template <class Struct, class Field, class ...PreviousFields>
struct StructDefinition<Struct, Field, PreviousFields...> {
    FieldDefinition<Struct, Field> field;
    StructDefinition<Struct, PreviousFields...> previous_fields;
    constexpr static std::size_t field_number = sizeof...(PreviousFields) + 1;

    const char* struct_name() const {
        return previous_fields.struct_name();
    }

    constexpr explicit StructDefinition(FieldDefinition<Struct, Field> field, StructDefinition<Struct, PreviousFields...> previous_fields)
        : field(field), previous_fields(previous_fields) {}

    template <class NewField>
    constexpr auto with_field(const char* field_name, MemberPtr<Struct, NewField> ptr) const {
        auto new_field = FieldDefinition<Struct, NewField>(field_name, ptr);
        return StructDefinition<Struct, NewField, Field, PreviousFields...>(new_field, *this);
    }

    template <class NewField>
    constexpr auto operator()(const char* field_name, MemberPtr<Struct, NewField> ptr) const {
        return with_field(field_name, ptr);
    }

    void serialize(kingw::ser::Serializer & serializer, const Struct & input) const {
        kingw::ser::Serializer::SerializeStruct state = serializer.serialize_struct(struct_name(), field_number);
        serialize_recurse(state, input);
        state.end();
    }

    void serialize_recurse(kingw::ser::Serializer::SerializeStruct & state, const Struct & input) const {
        previous_fields.serialize_recurse(state, input);
        field.serialize(state, input);
    }

    struct Visitor : public kingw::de::Visitor {
        Struct & output;
        StructDefinition defn;
        explicit Visitor(Struct & output, StructDefinition defn) : output(output), defn(defn) {}
        const char* expecting() const override { return defn.struct_name(); }
        void visit_map(kingw::de::Deserializer::MapAccess & accessor) override {
            std::size_t field_index = 0;
            FieldNameAccessor field_name_accessor(defn, field_index);
            while (accessor.has_next()) {
                accessor.next_key(field_name_accessor);
                defn.deserialize_field(accessor, field_index, output);
            }
        }
    };

    void deserialize(kingw::de::Deserializer & deserializer, Struct & output) const {
        const char* names[sizeof...(PreviousFields) + 1] = {};
        field_names(names);
        Visitor visitor(output, *this);
        deserializer.deserialize_struct(struct_name(), kingw::de::Deserializer::FieldNames(std::begin(names), std::end(names)), visitor);
    }

    void deserialize_field(kingw::de::Deserializer::MapAccess & accessor, std::size_t field_index, Struct & output) const {
        if (field_index == field_number) {
            field.deserialize(accessor, output);
        } else {
            previous_fields.deserialize_field(accessor, field_index, output);
        }
    }

    std::size_t index_of_field_name(const char* field_name, std::size_t len) const {
        std::size_t result = previous_fields.index_of_field_name(field_name, len);
        if (result == 0 && std::strcmp(field_name, field.name) == 0) {
            result = field_number;
        }
        return result;
    }

    void field_names(const char** names_arr) const {
        names_arr[field_number-1] = field.name;
        previous_fields.field_names(names_arr);
    }

    struct FieldNameAccessor : public kingw::de::Deserialize, public kingw::de::Visitor {
        StructDefinition defn;
        std::size_t & output;

        FieldNameAccessor(StructDefinition defn, std::size_t & output)
            : defn(defn), output(output) {}
        
        void deserialize(kingw::de::Deserializer & deserializer) override {
            deserializer.deserialize_string(*this);
        }

        kingw::serde::TypeTraits traits() const override {
            return kingw::serde::TypeTraits::of<std::size_t>();
        }

        const char* expecting() const override { return "struct field name"; }  // TODO: More precise?
        
        void visit_c_str(const char* key, std::size_t len) override {
            output = defn.index_of_field_name(key, len);
        }
        
        void visit_string(const std::string & key) override {
            visit_c_str(key.c_str(), key.size());
        }
    };
};

#define DERIVE_SERIALIZE(StructName, ...) \
template <> \
void kingw::ser::serialize<StructName>(kingw::ser::Serializer & serializer, const StructName & input) { \
    using Self = StructName; \
    auto defn = StructDefinition<StructName>(#StructName) __VA_ARGS__ ; \
    defn.serialize(serializer, input); \
}

#define DERIVE_DESERIALIZE(StructName, ...) \
template <> \
void kingw::de::deserialize<StructName>(kingw::de::Deserializer & deserializer, StructName & output) { \
    using Self = StructName; \
    auto defn = StructDefinition<StructName>(#StructName) __VA_ARGS__ ; \
    defn.deserialize(deserializer, output); \
}


#define DERIVE_SERDE(StructName, ...) \
DERIVE_SERIALIZE(StructName, __VA_ARGS__); \
DERIVE_DESERIALIZE(StructName, __VA_ARGS__);
