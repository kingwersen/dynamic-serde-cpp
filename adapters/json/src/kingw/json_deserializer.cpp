#include "kingw/json_deserializer.hpp"

#include "kingw/de/accessor.hpp"


namespace kingw {

JsonDeserializer::JsonDeserializationException::JsonDeserializationException(const char* message)
    : de::DeserializationException(message) { }

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
void JsonDeserializer::deserialize_any(de::Visitor & visitor) {
    throw JsonDeserializationException("deserialize_any() not implemented");
}
void JsonDeserializer::deserialize_bool(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_boolean()) {
        visitor.visit_bool(json);
    } else {
        throw JsonDeserializationException("json value was not boolean");
    }
}
void JsonDeserializer::deserialize_i8(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i8(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_i16(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i16(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_i32(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i32(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_i64(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_i64(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u8(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u8(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u16(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u16(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u32(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u32(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_u64(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_integer()) {
        visitor.visit_u64(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_f32(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_float() || json.is_number_integer()) {
        visitor.visit_f32(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_f64(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_number_float() || json.is_number_integer()) {
        visitor.visit_f64(json);
    } else {
        throw JsonDeserializationException("json value was not a number");
    }
}
void JsonDeserializer::deserialize_char(de::Visitor & visitor) {
    deserialize_string(visitor);  // nlohmann::json doesn't support char
}
void JsonDeserializer::deserialize_string(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_string()) {
        std::string value = json;
        visitor.visit_string(value.begin().base(), value.end().base());
    } else {
        throw JsonDeserializationException("json value was not a string");
    }
}
void JsonDeserializer::deserialize_seq(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_array()) {
        JsonSeqAccess seq(json);
        visitor.visit_seq(seq);
    } else {
        throw JsonDeserializationException("json value was not a sequence");
    }
}
void JsonDeserializer::deserialize_map(de::Visitor & visitor) {
    auto & json = json_stack.top();
    if (json.is_object()) {
        JsonMapAccess map(json);
        visitor.visit_map(map);
    } else {
        throw JsonDeserializationException("json value was not a map");
    }
}
void JsonDeserializer::deserialize_struct(
    const char* name,
    const FieldNames & field_names,
    de::Visitor & visitor)
{
    auto & json = json_stack.top();
    if (json.is_object()) {
        JsonStructAccess map(json, field_names);
        visitor.visit_map(map);
    } else {
        throw JsonDeserializationException("json value was not a struct");
    }
}

JsonDeserializer::JsonSeqAccess::JsonSeqAccess(nlohmann::json new_seq)
    : seq(std::move(new_seq)), iter(seq.begin()) { }
bool JsonDeserializer::JsonSeqAccess::has_next() {
    return iter != seq.end();
}
void JsonDeserializer::JsonSeqAccess::next_element(de::Deserialize & element) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(*iter);
        element.deserialize(deserializer);
        ++iter;
    } else {
        throw JsonDeserializationException("json end of sequence reached");
    }
}

JsonDeserializer::JsonMapAccess::JsonMapAccess(nlohmann::json new_map)
    : map(std::move(new_map)), items(map.items()), iter(items.begin()) { }
bool JsonDeserializer::JsonMapAccess::has_next() {
    return iter != items.end();
}
void JsonDeserializer::JsonMapAccess::next_key(de::Deserialize & key) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(std::string("\"") + iter.key() + "\"");
        key.deserialize(deserializer);
    } else {
        throw JsonDeserializationException("json end of map reached");
    }
}
void JsonDeserializer::JsonMapAccess::next_value(de::Deserialize & value) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(iter.value());
        value.deserialize(deserializer);
        ++iter;
    } else {
        throw JsonDeserializationException("json end of map reached");
    }
}
void JsonDeserializer::JsonMapAccess::next_entry(de::Deserialize & key, de::Deserialize & value) {
    next_key(key);
    next_value(value);
}

JsonDeserializer::JsonStructAccess::JsonStructAccess(nlohmann::json new_map, const FieldNames & field_names)
    : map(std::move(new_map)), field_names(field_names), iter(field_names.begin()) { }
bool JsonDeserializer::JsonStructAccess::has_next() {
    return iter != field_names.end();
}
void JsonDeserializer::JsonStructAccess::next_key(de::Deserialize & key) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(std::string("\"") + *iter + "\"");
        key.deserialize(deserializer);
    } else {
        throw JsonDeserializationException("json end of map reached");
    }
}
void JsonDeserializer::JsonStructAccess::next_value(de::Deserialize & value) {
    if (has_next()) {
        // TODO: Make efficient with pointers. Be careful with std::stack iterator invalidation.
        JsonDeserializer deserializer(map[*iter]);
        value.deserialize(deserializer);
        ++iter;
    } else {
        throw JsonDeserializationException("json end of map reached");
    }
}
void JsonDeserializer::JsonStructAccess::next_entry(de::Deserialize & key, de::Deserialize & value) {
    next_key(key);
    next_value(value);
}

}  // namespace kingw
