#pragma once

#include "kingw/serde/derive_common.hpp"
#include "kingw/de/deserializer.hpp"


#define KINGW_SERDE_IMPLEMENT_FIELDS_LIST(name, fn) #name,
#define KINGW_SERDE_IMPLEMENT_FIELDS_ENUM(name, fn) name,
#define KINGW_SERDE_IMPLEMENT_FIELDS_ENUM_CMP(name, fn) if (key == #name) { object = FieldsEnum::name; } else
#define KINGW_SERDE_IMPLEMENT_FIELDS_ACCESSORS(name, fn) auto name ## _accessor = kingw::de::accessor(fn);
#define KINGW_SERDE_IMPLEMENT_FIELDS_SWITCH(name, fn) case FieldsEnum::name: accessor.next_value(name ## _accessor); break;

#define DERIVE_DESERIALIZE(StructName, ...) \
enum class StructName ## FieldsEnum { \
    KINGW_SERDE_PP_MAP(KINGW_SERDE_IMPLEMENT_FIELDS_ENUM, __VA_ARGS__) \
}; \
template <> \
void kingw::de::deserialize<StructName ## FieldsEnum>(kingw::de::Deserializer & deserializer, StructName ## FieldsEnum & object) { \
    using FieldsEnum = StructName ## FieldsEnum; \
    struct StructName ## FieldsEnumVisitor : public kingw::de::Visitor { \
        explicit StructName ## FieldsEnumVisitor(FieldsEnum & object) : object(object) { } \
        const char * expecting() const override { return "'value1' or 'value2' or 's'"; } \
        void visit_string(const std::string & key) override { \
            KINGW_SERDE_PP_MAP(KINGW_SERDE_IMPLEMENT_FIELDS_ENUM_CMP, __VA_ARGS__) { \
                throw kingw::de::DeserializationException(std::string("unsupported key: ") + key); \
            } \
        } \
        FieldsEnum & object; \
    }; \
    StructName ## FieldsEnumVisitor visitor(object); \
    deserializer.deserialize_string(visitor); \
} \
template <> \
void kingw::de::deserialize<StructName>(kingw::de::Deserializer & deserializer, StructName & object) { \
    using FieldsEnum = StructName ## FieldsEnum; \
    struct StructName ## Visitor : public kingw::de::Visitor { \
        explicit StructName ## Visitor(StructName & object) : object(object) { } \
        const char * expecting() const override { return "struct " #StructName; } \
        void visit_map(de::Deserializer::MapAccess & accessor) override { \
            KINGW_SERDE_PP_MAP(KINGW_SERDE_IMPLEMENT_FIELDS_ACCESSORS, __VA_ARGS__); \
            while (accessor.has_next()) { \
                FieldsEnum field; \
                kingw::de::Accessor<FieldsEnum> field_accessor(field); \
                accessor.next_key(field_accessor); \
                switch (field) { \
                KINGW_SERDE_PP_MAP(KINGW_SERDE_IMPLEMENT_FIELDS_SWITCH, __VA_ARGS__); \
                default: throw kingw::de::DeserializationException("unsupported key"); \
                } \
            } \
        } \
        StructName & object; \
    }; \
    StructName ## Visitor visitor(object); \
    const auto fields = { KINGW_SERDE_PP_MAP(KINGW_SERDE_IMPLEMENT_FIELDS_LIST, __VA_ARGS__) }; \
    deserializer.deserialize_struct(#StructName, fields, visitor); \
}
