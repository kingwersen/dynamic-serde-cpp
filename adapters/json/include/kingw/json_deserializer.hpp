#pragma once

#include <stack>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "kingw/de/deserializer.hpp"


namespace kingw {

class JsonDeserializer :
    public de::Deserializer
{
public:
    class JsonDeserializationException : public de::DeserializationException {
    public:
        explicit JsonDeserializationException(serde::string_view message);
    };

    template <class T>
    static void from_string(const T & item, const std::string & contents) {
        JsonDeserializer deserializer(contents);
        de::deserialize(deserializer, item);
    }

    template <class T>
    static T from_string(const std::string & contents) {
        T item;
        JsonDeserializer deserializer(contents);
        de::deserialize(deserializer, item);
        return item;
    }

    explicit JsonDeserializer(const nlohmann::json & contents);
    explicit JsonDeserializer(const std::string & contents);
    bool is_human_readable() const override;

    // Basic Types
    void deserialize_any(de::Visitor & visitor) override;
    void deserialize_bool(de::Visitor & visitor) override;
    void deserialize_i8(de::Visitor & visitor) override;
    void deserialize_i16(de::Visitor & visitor) override;
    void deserialize_i32(de::Visitor & visitor) override;
    void deserialize_i64(de::Visitor & visitor) override;
    void deserialize_u8(de::Visitor & visitor) override;
    void deserialize_u16(de::Visitor & visitor) override;
    void deserialize_u32(de::Visitor & visitor) override;
    void deserialize_u64(de::Visitor & visitor) override;
    void deserialize_f32(de::Visitor & visitor) override;
    void deserialize_f64(de::Visitor & visitor) override;
    void deserialize_char(de::Visitor & visitor) override;
    void deserialize_string(de::Visitor & visitor) override;
    void deserialize_seq(de::Visitor & visitor) override;
    void deserialize_map(de::Visitor & visitor) override;
    void deserialize_struct(
        serde::string_view name,
        const FieldNames & field_names,
        de::Visitor & visitor) override;

    class JsonSeqAccess : public de::Deserializer::SeqAccess
    {
    public:
        explicit JsonSeqAccess(nlohmann::json seq);
        bool has_next() override;
        void next_element(de::Deserialize & element) override;
    private:
        nlohmann::json seq;
        decltype(seq.begin()) iter;
    };

    class JsonMapAccess : public de::Deserializer::MapAccess
    {
    public:
        explicit JsonMapAccess(nlohmann::json map);
        bool has_next() override;
        void next_key(de::Deserialize & key) override;
        void next_value(de::Deserialize & value) override;
        void next_entry(de::Deserialize & key, de::Deserialize & value) override;
    private:
        nlohmann::json map;
        decltype(nlohmann::json().items()) items;
        decltype(nlohmann::json().items().begin()) iter;
    };

    class JsonStructAccess : public de::Deserializer::MapAccess
    {
    public:
        explicit JsonStructAccess(nlohmann::json map, const FieldNames & fields);
        bool has_next() override;
        void next_key(de::Deserialize & key) override;
        void next_value(de::Deserialize & value) override;
        void next_entry(de::Deserialize & key, de::Deserialize & value) override;
    private:
        nlohmann::json map;
        const FieldNames & field_names;
        decltype(field_names.begin()) iter;
    };

private:
    std::stack<nlohmann::json> json_stack;
};

}  // namespace kingw
