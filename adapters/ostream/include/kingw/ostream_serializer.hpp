#pragma once

#include <string>
#include <sstream>

#include "kingw/serde/serialize.hpp"


namespace kingw {

class OStreamSerializer :
    public ser::Serializer
{
public:
    template <class T>
    static std::string to_string(const T & item) {
        std::ostringstream stream;
        OStreamSerializer serializer(stream);
        ser::serialize(serializer, item);
        return stream.str();
    }

    template <class T>
    void serialize(const T & item) {
        ser::serialize(*this, item);
    }

    explicit OStreamSerializer(std::ostream & stream);

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
    void serialize_seq_element(const ser::Dynamic & accessor) override;
    void serialize_seq_end() override;

    // Maps
    // Use serialize_map() instead.
    void serialize_map_begin() override;
    void serialize_map_key(const ser::Dynamic & accessor) override;
    void serialize_map_value(const ser::Dynamic & accessor) override;
    void serialize_map_end() override;

    // Structs
    // Use serialize_struct() instead.
    void serialize_struct_begin() override;
    void serialize_struct_field(const ser::Dynamic & accessor, const char * name) override;
    void serialize_struct_end() override;

private:
    std::ostream & stream;
};

}
