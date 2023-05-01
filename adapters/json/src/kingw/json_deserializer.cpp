#include "kingw/json_deserializer.hpp"

#include "kingw/serde/accessor.hpp"


namespace kingw {

JsonDeserializer::DeserializationException::DeserializationException(const std::string & message)
    : ser::DeserializeException(message) { }

JsonDeserializer::JsonDeserializer(const nlohmann::json & contents) {
    json_stack.push({ contents });
}

JsonDeserializer::JsonDeserializer(const std::string & contents) {
    json_stack.push({ nlohmann::json::parse(contents) });
}

bool JsonDeserializer::is_human_readable() const {
    return true;
}

// Basic Types
void JsonDeserializer::deserialize_any(ser::Visitor & visitor) {
    throw DeserializationException("deserialize_any() not implemented");
}
void JsonDeserializer::deserialize_bool(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_boolean()) {
        visitor.visit_bool(json);
    } else {
        throw DeserializationException("json value was not boolean");
    }
}
void JsonDeserializer::deserialize_i8(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i8(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_i16(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i16(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_i32(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i32(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_i64(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i64(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u8(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u8(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u16(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u16(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u32(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u32(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u64(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u64(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_f32(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_float() || json.is_number_integer()) {
        visitor.visit_f32(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_f64(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_float() || json.is_number_integer()) {
        visitor.visit_f64(json);
    } else {
        throw DeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_char(ser::Visitor & visitor) {
    deserialize_string(visitor);  // nlohmann::json doesn't support char
}
void JsonDeserializer::deserialize_string(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_string()) {
        visitor.visit_string(json);
    } else {
        throw DeserializationException("json value was not a string");
    }
}
void JsonDeserializer::deserialize_seq(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_array()) {
        JsonSeqAccess seq(json);
        visitor.visit_seq(seq);
    } else {
        throw DeserializationException("json value was not a sequence");
    }
}
void JsonDeserializer::deserialize_map(ser::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_object()) {
        JsonMapAccess map(json);
        visitor.visit_map(map);
    } else {
        throw DeserializationException("json value was not a map");
    }
}

JsonDeserializer::JsonSeqAccess::JsonSeqAccess(nlohmann::json new_seq)
    : seq(std::move(new_seq)), iter(seq.begin()) { }
bool JsonDeserializer::JsonSeqAccess::has_next() {
    return iter != seq.end();
}
void JsonDeserializer::JsonSeqAccess::next_element(ser::Dynamic & element) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(*iter);
        element.deserialize(deserializer);
        ++iter;
    } else {
        throw DeserializationException("json end of sequence reached");
    }
}

JsonDeserializer::JsonMapAccess::JsonMapAccess(nlohmann::json new_map)
    : map(std::move(new_map)), items(map.items()), iter(items.begin()) { }
bool JsonDeserializer::JsonMapAccess::has_next() {
    return iter != items.end();
}
void JsonDeserializer::JsonMapAccess::next_key(ser::Dynamic & key) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(std::string("\"") + iter.key() + "\"");
        key.deserialize(deserializer);
    } else {
        throw DeserializationException("json end of map reached");
    }
}
void JsonDeserializer::JsonMapAccess::next_value(ser::Dynamic & value) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(iter.value());
        value.deserialize(deserializer);
        ++iter;
    } else {
        throw DeserializationException("json end of map reached");
    }
}
void JsonDeserializer::JsonMapAccess::next_entry(ser::Dynamic & key, ser::Dynamic & value) {
    next_key(key);
    next_value(value);
}

}
