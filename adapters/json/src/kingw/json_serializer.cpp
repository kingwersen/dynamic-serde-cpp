#include "kingw/json_serializer.hpp"


namespace kingw {

JsonSerializer::SerializationException::SerializationException(const std::string & str)
    : std::runtime_error(str) { }

JsonSerializer::JsonSerializer(std::ostream & stream, bool pretty)
    : stream(stream), pretty(pretty) { }

void JsonSerializer::serialize_bool(bool value) {
    stream << value;
}
void JsonSerializer::serialize_i8(std::int8_t value) {
    stream << value;
}
void JsonSerializer::serialize_i16(std::int16_t value) {
    stream << value;
}
void JsonSerializer::serialize_i32(std::int32_t value) {
    stream << value;
}
void JsonSerializer::serialize_i64(std::int64_t value) {
    stream << value;
}
void JsonSerializer::serialize_u8(std::uint8_t value) {
    stream << value;
}
void JsonSerializer::serialize_u16(std::uint16_t value) {
    stream << value;
}
void JsonSerializer::serialize_u32(std::uint32_t value) {
    stream << value;
}
void JsonSerializer::serialize_u64(std::uint64_t value) {
    stream << value;
}
void JsonSerializer::serialize_f32(float value) {
    stream << value;
}
void JsonSerializer::serialize_f64(double value) {
    stream << value;
}
void JsonSerializer::serialize_char(char value) {
    stream << value;
}
void JsonSerializer::serialize_c_str(const char * value) {
    stream << "\"" << value << "\"";
}
void JsonSerializer::serialize_string(const std::string & value) {
    stream << "\"" << value << "\"";
}


///
/// Sequences 
///

void JsonSerializer::serialize_seq_begin() {
    if (pretty) {
        indents += 1;
    }
    stream << "[";
    comma_tracker.push(true);
}

void JsonSerializer::serialize_seq_element(const ser::Dynamic & accessor) {
    if (comma_tracker.top()) {
        // Do not print a comma before the first item
        comma_tracker.top() = false;
    } else {
        stream << ",";
    }
    if (pretty) {
        stream << "\n";
        for (int i = 0; i < indents; ++i) { stream << "    "; }
    }
    accessor.serialize(*this);
}

void JsonSerializer::serialize_seq_end() {
    if (pretty) {
        indents -= 1;
        stream << "\n";
        for (int i = 0; i < indents; ++i) { stream << "    "; }
    }
    stream << "]";
    comma_tracker.pop();
}


///
/// Maps 
///

void JsonSerializer::serialize_map_begin() {
    stream << "{";
    comma_tracker.push(true);
}

void JsonSerializer::serialize_map_key(const ser::Dynamic & accessor) {
    if (!accessor.traits().is_string) {
        throw SerializationException("JsonSerializer map key is not a string");
    }

    // Print a comma only after the first key-value-pair.
    if (comma_tracker.top()) {
        comma_tracker.top() = false;
    } else {
        stream << ",";
    }

    // If pretty, print indents.
    if (pretty) {
        stream << "\n";
        for (int i = 0; i < indents; ++i) { stream << "    "; }
    }

    // Serialize the key.
    accessor.serialize(*this);
}

void JsonSerializer::serialize_map_value(const ser::Dynamic & accessor) {
    // Serialize the value.
    stream << ":";
    if (pretty) {
        stream << " ";
    }
    accessor.serialize(*this);
}

void JsonSerializer::serialize_map_end() {
    // Update indentation, reset comma to previous level.
    comma_tracker.pop();
    if (pretty) {
        indents -= 1;
        stream << "\n";
        for (int i = 0; i < indents; ++i) { stream << "    "; }
    }
    stream << "}";
}


///
/// Structs 
///

void JsonSerializer::serialize_struct_begin() {
    if (pretty) {
        indents += 1;
    }
    stream << "{";
    comma_tracker.push(true);
}

void JsonSerializer::serialize_struct_field(const ser::Dynamic & accessor, const char * name) {
    // Print a comma only after the first key-value-pair.
    if (comma_tracker.top()) {
        comma_tracker.top() = false;
    } else {
        stream << ",";
    }

    // If pretty, print indents.
    if (pretty) {
        stream << "\n";
        for (int i = 0; i < indents; ++i) { stream << "    "; }
    }

    // Print the name of the field and then the field.
    serialize_c_str(name);
    stream << ":";
    if (pretty) {
        stream << " ";
    }
    accessor.serialize(*this);
}

void JsonSerializer::serialize_struct_end() {
    // Update indentation, reset comma to previous level.
    comma_tracker.pop();
    if (pretty) {
        indents -= 1;
        stream << "\n";
        for (int i = 0; i < indents; ++i) { stream << "    "; }
    }
    stream << "}";
}

}  // namespace kingw
