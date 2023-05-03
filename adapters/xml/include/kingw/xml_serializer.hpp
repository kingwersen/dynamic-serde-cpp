#pragma once

#include <sstream>
#include <stack>

#include "kingw/ser/serializer.hpp"


namespace kingw {

class XmlSerializer :
    public ser::Serializer
{
public:
    class SerializationException : public std::runtime_error {
    public:
        explicit SerializationException(const std::string & str);
    };

    template <class T>
    static std::string to_string(const T & item) {
        std::ostringstream stream;
        XmlSerializer serializer(stream);
        ser::serialize(serializer, item);
        return stream.str();
    }

    template <class T>
    void serialize(const T & item) {
        ser::serialize(*this, item);
    }

    explicit XmlSerializer(std::ostream & stream);
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
    void seq_begin() override;
    void seq_serialize_element(const ser::Serialize & element) override;
    void seq_end() override;

    // Maps
    // Use serialize_map() instead.
    void map_begin() override;
    void map_serialize_key(const ser::Serialize & key) override;
    void map_serialize_value(const ser::Serialize & value) override;
    void map_serialize_entry(const ser::Serialize & key, const ser::Serialize & value) override;
    void map_end() override;

    // Structs
    // Use serialize_struct() instead.
    void struct_begin() override;
    void struct_serialize_field(const char * name, const ser::Serialize & accessor) override;
    void struct_skip_field(const char * name) override;
    void struct_end() override;

private:
    std::ostream & stream;
    std::stack<std::string> open_tags;
};

}  // namespace kingw
