#pragma once

#include "kingw/de/deserializer.hpp"


namespace kingw {
namespace serde_sprintf {

class SPrintfDeserializer :
    public de::Deserializer
{
public:
    class SPrintfDeserializationException : public de::DeserializationException {
    public:
        explicit SPrintfDeserializationException(serde::string_view message);
    };

    SPrintfDeserializer(serde::string_view input, bool human_readable = true);
    bool is_human_readable() const override;
    const char* last_end() const;

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

    class SPrintfSeqAccess : public de::Deserializer::SeqAccess
    {
    public:
        explicit SPrintfSeqAccess(SPrintfDeserializer & parent);
        bool has_next() override;
        void next_element(de::Deserialize & element) override;
    private:
        SPrintfDeserializer & parent;
        unsigned index;
        unsigned count;
    };

    class SPrintfMapAccess : public de::Deserializer::MapAccess
    {
    public:
        explicit SPrintfMapAccess(SPrintfDeserializer & parent);
        bool has_next() override;
        void next_key(de::Deserialize & key) override;
        void next_value(de::Deserialize & value) override;
        void next_entry(de::Deserialize & key, de::Deserialize & value) override;
    private:
        SPrintfDeserializer & parent;
        unsigned index;
        unsigned count;
    };

protected:
    serde::string_view next_delimited_string();

private:
    serde::string_view buffer;
    const char* last_end_;
    bool human_readable;
};

template <class T>
const char* from_string(T & output, serde::string_view input, bool human_readable = true) {
    SPrintfDeserializer deserializer(input, human_readable);
    de::deserialize(deserializer, output);
    return deserializer.last_end();
}

}  // namespace serde_sprintf
}  // namespace kingw
