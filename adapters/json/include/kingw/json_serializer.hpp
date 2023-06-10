#pragma once

#include <stack>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "kingw/ser/serializer.hpp"


namespace kingw {

class JsonSerializer :
    public ser::Serializer
{
public:
    class JsonSerializationException : public ser::SerializationException {
    public:
        explicit JsonSerializationException(serde::string_view message);
    };

    template <class T>
    static std::string to_string(const T & item) {
        JsonSerializer serializer;
        ser::serialize(serializer, item);
        return serializer.dump();
    }

    JsonSerializer();
    bool is_human_readable() const override;
    std::string dump() const;

    // Basic Types
    void serialize_bool(bool value) override;
    void serialize_i8(std::int8_t value) override;
    void serialize_i16(std::int16_t value) override;
    void serialize_i32(std::int32_t value) override;
    void serialize_i64(std::int64_t value) override;
    void serialize_u8(std::uint8_t value) override;
    void serialize_u16(std::uint16_t value) override;
    void serialize_u32(std::uint32_t value) override;
    void serialize_u64(std::uint64_t value) override;
    void serialize_f32(float value) override;
    void serialize_f64(double value) override;
    void serialize_char(char value) override;
    void serialize_string(serde::string_view value) override;

protected:
    // Lists/Sequences
    // Use serialize_seq() instead.
    void seq_begin(std::size_t len) override;
    void seq_serialize_element(const ser::Serialize & accessor) override;
    void seq_end() override;

    // Maps
    // Use serialize_map() instead.
    void map_begin(std::size_t len) override;
    void map_serialize_key(const ser::Serialize & key) override;
    void map_serialize_value(const ser::Serialize & value) override;
    void map_serialize_entry(const ser::Serialize & key, const ser::Serialize & value) override;
    void map_end() override;

    // Structs
    // Use serialize_struct() instead.
    void struct_begin(serde::string_view name, std::size_t len) override;
    void struct_serialize_field(serde::string_view name, const ser::Serialize & accessor) override;
    void struct_skip_field(serde::string_view name) override;
    void struct_end() override;

private:
    std::stack<nlohmann::json> json_stack;
    std::string current_map_key;
};

}  // namespace kingw
