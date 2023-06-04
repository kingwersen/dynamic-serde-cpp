#pragma once

#include "kingw/de/deserializer.hpp"


namespace kingw {

class SPrintfDeserializer :
    public de::Deserializer
{
public:
    class SPrintfDeserializationException : public de::DeserializationException {
    public:
        explicit SPrintfDeserializationException(const char* message);
    };

    template <class T>
    static void from_buffer(T & item, const char * buffer, std::size_t len, bool human_readable = true) {
        SPrintfDeserializer deserializer(buffer, len, human_readable);
        de::deserialize(deserializer, item);
    }

    template <class T, std::size_t N>
    static void from_buffer(T & item, const char (&buffer)[N], bool human_readable = true) {
        SPrintfDeserializer deserializer(buffer, N, human_readable);
        de::deserialize(deserializer, item);
    }

    SPrintfDeserializer(const char * buffer, std::size_t len, bool human_readable = true);
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
        const char* name,
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
    struct DelimitedString {
        const char* buffer;
        std::size_t len;
    };
    DelimitedString next_delimited_string();

private:
    const char* buffer;
    const char* buffer_end;
    bool human_readable;
};

}  // namespace kingw
