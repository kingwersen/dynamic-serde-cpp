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
    class SerializationException : public std::runtime_error {
    public:
        explicit SerializationException(const std::string & str);
    };

    template <class T>
    static std::string to_string(const T & item) {
        JsonSerializer serializer;
        ser::serialize(serializer, item);
        return serializer.dump();
    }

    JsonSerializer();
    std::string dump() const;
    bool is_human_readable() const override;

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
    void serialize_c_str(const char * value) override;
    void serialize_string(const std::string & value) override;

protected:
    // Lists/Sequences
    // Use serialize_seq() instead.
    void serialize_seq_begin() override;
    void serialize_seq_element(const ser::Serialize & accessor) override;
    void serialize_seq_end() override;

    // Maps
    // Use serialize_map() instead.
    void serialize_map_begin() override;
    void serialize_map_key(const ser::Serialize & accessor) override;
    void serialize_map_value(const ser::Serialize & accessor) override;
    void serialize_map_end() override;

    // Structs
    // Use serialize_struct() instead.
    void serialize_struct_begin() override;
    void serialize_struct_field(const ser::Serialize & accessor, const char * name) override;
    void serialize_struct_end() override;

private:
    std::stack<nlohmann::json> json_stack;
    std::string current_map_key;
};

}  // namespace kingw
