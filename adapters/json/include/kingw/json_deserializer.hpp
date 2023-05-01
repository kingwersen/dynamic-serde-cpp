#pragma once

#include <sstream>
#include <stack>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "kingw/serde/deserialize.hpp"


namespace kingw {

class JsonDeserializer :
    public ser::Deserializer
{
public:
    class DeserializationException : public ser::DeserializeException {
    public:
        explicit DeserializationException(const std::string & message);
    };

    template <class T>
    static void from_string(const T & item, const std::string & contents) {
        JsonDeserializer deserializer(contents);
        ser::deserialize(deserializer, item);
    }

    template <class T>
    static T from_string(const std::string & contents) {
        T item;
        JsonDeserializer deserializer(contents);
        ser::deserialize(deserializer, item);
        return item;
    }

    explicit JsonDeserializer(const nlohmann::json & contents);
    explicit JsonDeserializer(const std::string & contents);
    bool is_human_readable() const override;

    // Basic Types
    void deserialize_any(ser::Visitor & visitor) override;
    void deserialize_bool(ser::Visitor & visitor) override;
    void deserialize_i8(ser::Visitor & visitor) override;
    void deserialize_i16(ser::Visitor & visitor) override;
    void deserialize_i32(ser::Visitor & visitor) override;
    void deserialize_i64(ser::Visitor & visitor) override;
    void deserialize_u8(ser::Visitor & visitor) override;
    void deserialize_u16(ser::Visitor & visitor) override;
    void deserialize_u32(ser::Visitor & visitor) override;
    void deserialize_u64(ser::Visitor & visitor) override;
    void deserialize_f32(ser::Visitor & visitor) override;
    void deserialize_f64(ser::Visitor & visitor) override;
    void deserialize_char(ser::Visitor & visitor) override;
    void deserialize_string(ser::Visitor & visitor) override;
    void deserialize_seq(ser::Visitor & visitor) override;
    void deserialize_map(ser::Visitor & visitor) override;

    class JsonSeqAccess : public ser::Deserializer::SeqAccess
    {
    public:
        explicit JsonSeqAccess(nlohmann::json seq);
        bool has_next();
        void next_element(ser::Dynamic & element);
    private:
        nlohmann::json seq;
        decltype(seq.begin()) iter;
    };

    class JsonMapAccess : public ser::Deserializer::MapAccess
    {
    public:
        explicit JsonMapAccess(nlohmann::json map);
        bool has_next() override;
        void next_key(ser::Dynamic & key) override;
        void next_value(ser::Dynamic & value) override;
        void next_entry(ser::Dynamic & key, ser::Dynamic & value) override;
    private:
        nlohmann::json map;
        decltype(nlohmann::json().items()) items;
        decltype(nlohmann::json().items().begin()) iter;
    };

private:
    std::stack<nlohmann::json> json_stack;
};

}  // namespace kingw
