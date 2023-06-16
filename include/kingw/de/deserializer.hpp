#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

#include "kingw/de/deserialize.hpp"
#include "kingw/serde/string_view.hpp"


namespace kingw {
namespace de {

/// @brief Common exception class that should be used for deserialization exceptions
class DeserializationException : public std::runtime_error {
public:
    /// @brief DeserializationException Constructor
    /// @param message Cause of the exception
    explicit DeserializationException(serde::string_view message);
};

// Forward-declare from later in this file
class Visitor;

/// @brief Base class for a deserializer that processes some input data
///
/// A deserializer implementation should accept input data in some form,
/// such as from a string. It will gradually parse the contents of the
/// data based on the order of `deserialize_*()` functions called.
///
/// The order of `deserialize_*()` functions called is usually defined by
/// the `de::deserialize<T>()` free function in "kingw/de/deserialize.hpp".
/// Each type <T> can have its own implementation based on its structure.
/// There exist implementations for numbers, strings, std::vector, and
/// std::map, but you will need to write custom implementations for your
/// own classes.
///
/// In short, implement `de::deserialize<T>()` for your class and invoke
/// the function with your preferred `Deserializer` implementation.
///
/// https://serde.rs/impl-deserialize.html
/// https://serde.rs/deserialize-struct.html
/// https://docs.rs/serde/latest/serde/de/trait.Deserializer.html
class Deserializer
{
public:
    /// @brief Helper class for listing field names for deserialize_struct()
    ///
    /// Deserializer::deserialize_struct() needs to take a list of field names
    /// as a parameter. The parameter cannot be a template type because the
    /// function is polymorphic, and I do not want to dynamically allocate memory.
    ///
    /// I really wanted to use std::initializer_list<const char*>,
    /// but apparently I can't construct one to point to an existing array.
    /// They can ONLY be constructed using the special brace syntax.
    ///
    /// So, this structure adds a layer of abstraction. It acts like a
    /// standard initializer list except that it has an additional constructor
    /// for begin and end iterators.
    class FieldNames
    {
    public:
        /// @brief Alias for an iterator to a collection of (serde::string_view)
        using Iterator = const serde::string_view*;

        /// @brief Initializer List Constructor (implicit)
        /// @param init List of (const char*)
        FieldNames(std::initializer_list<serde::string_view> init);

        /// @brief Iterator Pair Constructor
        /// @param begin Forward iter (begin) to a collection of (const char*)
        /// @param end Forward iter (end) to a collection of (const char*)
        FieldNames(Iterator begin, Iterator end);

        /// @brief Iterator to the beginning of the collection of field names
        /// @return Iterator to the beginning of the collection of field names
        Iterator begin() const;

        /// @brief Iterator to the end of the collection of field names
        /// @return Iterator to the end of the collection of field names
        Iterator end() const;

    private:
        /// @brief Copy of an initializer list (optional)
        ///
        /// This field only exists to extend the lifetime of the init list when
        /// using the corresponding constructor. Users will use begin()
        /// and end() instead and will not interact with the list itself.
        ///
        /// If using the Iterator pair constructor, this field will be unused.
        std::initializer_list<serde::string_view> list;  // maybe unused

        /// @brief Iterator to the first field name
        Iterator begin_;

        /// @brief Iterator to the end of the field names collection
        Iterator end_;
    };

    /// @brief Deserializer Destructor
    virtual ~Deserializer() = default;

    /// @brief Whether this deserializes from a readable format.
    ///
    /// This refers to the format of the string contents (for example)
    /// that the deserializer is processing.
    /// - JSON/YAML/XML/HTML etc. are human readable.
    /// - Binary/Protobuf etc. are not human readable.
    ///
    /// Certain types may choose to serialize/deserialize differently
    /// for readable and non-readable formats. For example, a timestamp
    /// may be in the form of a string ("%Y-%m-%d %H:%M:%S") or more
    /// consisely as a 64-bit unsigned integer.
    ///
    /// If this deserializer is human readable, then the type may use
    /// deserialize_string(). If not, then it may use deserialize_u64().
    ///
    /// @return True/False if the format is human readable.
    virtual bool is_human_readable() const = 0;

    /// @brief Various deserialize functions
    ///
    /// When any `deserialize_*()` is called, the deserializer
    /// should attempt to deserialize whatever type is requested
    /// from the input data.
    /// However, it may end up with a different type.
    /// Whatever type of value was deserialized should be provided
    /// to the corresponding `visit_*()` function in the visitor.
    ///
    /// The purpose of the visitor is to figure out the difference
    /// between what was deserialized and what was expected. It will
    /// either translate and save the data if possible or throw an
    /// exception if not. See `Visitor` comments below for more
    /// info on visitor usage and an exmaple.
    ///
    /// @param visitor Handles the deserialized value
    virtual void deserialize_any(Visitor & visitor) = 0;
    virtual void deserialize_bool(Visitor & visitor) = 0;
    virtual void deserialize_i8(Visitor & visitor) = 0;
    virtual void deserialize_i16(Visitor & visitor) = 0;
    virtual void deserialize_i32(Visitor & visitor) = 0;
    virtual void deserialize_i64(Visitor & visitor) = 0;
    virtual void deserialize_u8(Visitor & visitor) = 0;
    virtual void deserialize_u16(Visitor & visitor) = 0;
    virtual void deserialize_u32(Visitor & visitor) = 0;
    virtual void deserialize_u64(Visitor & visitor) = 0;
    virtual void deserialize_f32(Visitor & visitor) = 0;
    virtual void deserialize_f64(Visitor & visitor) = 0;
    virtual void deserialize_char(Visitor & visitor) = 0;
    virtual void deserialize_string(Visitor & visitor) = 0;
    virtual void deserialize_seq(Visitor & visitor) = 0;
    virtual void deserialize_map(Visitor & visitor) = 0;
    virtual void deserialize_struct(serde::string_view struct_name,
        const FieldNames & field_names, 
        Visitor & visitor) = 0;

    /// @brief Provides a Visitor access to each element of a sequence
    ///
    /// This is an abstract, untyped representation of a list/sequence
    /// in the input data.
    ///
    /// When `deserialize_seq()` is called, it informs the deserializer
    /// that the next data it finds should be a list/sequence.
    /// The deserializer creates an instance of `SeqAccess` to represent
    /// whatever abstract list it finds and provides it to the visitor
    /// so that it may process the data.
    ///
    /// Using the member variables of this class, the visitor can
    /// allocate output variables and pass them to `next_element()`
    /// so that the deserializer can deserialize them one by one.
    ///
    /// https://docs.rs/serde/latest/serde/de/trait.SeqAccess.html
    class SeqAccess
    {
    public:
        /// @brief SeqAccess Dstructor
        virtual ~SeqAccess() = default;

        /// @brief Returns true if this abstract list has more elements
        /// @return True/False if this abstract list has more elements
        virtual bool has_next() = 0;

        /// @brief Requests to deserialize the next element in the list
        ///
        /// The `Deserialize` element parameter knows how to deserialize
        /// abstract data into itself. The deserializer uses it on
        /// the next item in the list.
        ///
        /// @param element Output location
        virtual void next_element(Deserialize & element) = 0;
    };

    /// @brief Provides a Visitor access to each element of a map
    ///
    /// This is an abstract, untyped representation of a map
    /// in the input data.
    ///
    /// When `deserialize_map()` is called, it informs the deserializer
    /// that the next data it finds should be a map.
    /// The deserializer creates an instance of `MapAccess` to represent
    /// whatever abstract map it finds and provides it to the visitor
    /// so that it may process the data.
    ///
    /// Using the member variables of this class, the visitor can
    /// allocate output variables and pass them to `next_*()`
    /// so that the deserializer can deserialize the map entries one by one.
    ///
    /// https://docs.rs/serde/latest/serde/de/trait.MapAccess.html
    class MapAccess
    {
    public:
        /// @brief MapAccess Destructor
        virtual ~MapAccess() = default;

        /// @brief Returns true if this abstract map has more entries
        /// @return True/False if this abstract map has more entries
        virtual bool has_next() = 0;

        /// @brief Requests to deserialize the key for the current map entry
        ///
        /// The `Deserialize` key parameter knows how to deserialize
        /// abstract data into itself. The deserializer uses it on the map key.
        ///
        /// Use `next_value()` to move onto the next entry.
        ///
        /// `next_key() + next_value()` vs `next_entry()` should be
        /// functionally equivalent. You can use whichever is easier.
        ///
        /// @param key Output location
        virtual void next_key(Deserialize & key) = 0;

        /// @brief Requests to deserialize the value for the current map entry
        ///
        /// The `Deserialize` value parameter knows how to deserialize
        /// abstract data into itself. The deserializer uses it on the map value.
        ///
        /// After this call, the `MapAccess` moves onto the next entry.
        ///
        /// `next_key() + next_value()` vs `next_entry()` should be
        /// functionally equivalent. You can use whichever is easier.
        ///
        /// @param value Output location
        virtual void next_value(Deserialize & value) = 0;

        /// @brief Requests to deserialize the current map entry
        ///
        /// The `Deserialize` key and value parameters know how to deserialize
        /// abstract data into themselves. The deserializer uses it on the map entry.
        ///
        /// After this call, the `MapAccess` moves onto the next entry.
        ///
        /// `next_key() + next_value()` vs `next_entry()` should be
        /// functionally equivalent. You can use whichever is easier.
        ///
        /// @param key Output location
        /// @param value Output location
        virtual void next_entry(Deserialize & key, Deserialize & value) = 0;
    };
};

/// @brief Base class for a visitor that analyzes `Deserializer` results.
///
/// A basic Visitor implementation will hold a reference to the variable
/// (from the final data structure) that it will assign a value to.
/// It will assign a value based on which visitor function is invoked.
///
/// See the function comments below for more info.
class Visitor
{
public:
    /// @brief Exception class thrown when a visitor call is not implemented
    class NotImplementedException : public DeserializationException {
    public:
        /// @brief NotImplementedException Constructor
        /// @param message Cause of the exception
        explicit NotImplementedException(serde::string_view message);
    };

    /// @brief Visitor Destructor
    virtual ~Visitor() = default;

    /// @brief Human-readable hint at what the visitor is expecting
    /// @return A string literal
    virtual const char* expecting() const = 0;

    /// @brief Various visitor functions
    ///
    /// To deserialize a value, we first call `deserializer.deserialize_*()`.
    /// This initial call gives the deserializer a "hint" of what it will find.
    /// The deserializer will then deserialize the data into this format
    /// to the best of its ability, but *it might end up with something different*.
    /// The deserializer will then invoke one of the following functions
    /// based on what it found so that the visitor can save it.
    ///
    /// For example, say I deserialize a 32-bit integer from a human-readable
    /// format (like JSON) which stores integers in the form of a string.
    /// When I call `deserializer.deserialize_i32()` to get that data, the
    /// deserializer may correctly deserialize into an integer, but for
    /// simplicity it may prefer to deserialize into a 64-bit integer instead.
    /// It will then call `visitor.visit_i64()` instead of `visitor.visit_i32()`.
    /// In this case, it is finally the visitor's job to verify that the 64-bit
    /// integer value that was *actually* deserialized fits in the 32-bit integer
    /// memory location that it will be stored in. It will save the integer
    /// if it does or throw an exception if it does not.
    ///
    /// There are already visitors for all the integral types that handle
    /// integer, unsigned, and float conversion, etc. You do not need to
    /// write these yourself. They are used by default when you use
    /// `de::deserialize<T>()`. However, you will need to write visitors
    /// for custom formats like timestamps.
    ///
    /// The default implementation of each visitor function is to throw
    /// Visitor::NotImplementedException.
    ///
    /// https://serde.rs/impl-deserialize.html
    /// https://serde.rs/deserialize-struct.html
    /// https://docs.rs/serde/latest/serde/de/trait.Visitor.html
    ///
    /// @param value Deserialized value from the deserializer
    virtual void visit_bool(bool value);
    virtual void visit_i8(std::int8_t value);
    virtual void visit_i16(std::int16_t value);
    virtual void visit_i32(std::int32_t value);
    virtual void visit_i64(std::int64_t value);
    virtual void visit_u8(std::uint8_t value);
    virtual void visit_u16(std::uint16_t value);
    virtual void visit_u32(std::uint32_t value);
    virtual void visit_u64(std::uint64_t value);
    virtual void visit_f32(float value);
    virtual void visit_f64(double value);
    virtual void visit_char(char value);
    virtual void visit_string(serde::string_view value);
    virtual void visit_seq(Deserializer::SeqAccess & value);
    virtual void visit_map(Deserializer::MapAccess & value);
};


/// @brief Generic `Deserialize` implementation that defers to `de::deserialize<T>()`.
///
/// This Serde implementation uses dynamic dispatch to reduce compilation
/// overhead. If your class does not inherit from `Deserialize`, then you
/// will be required to use this class to use a `Deserializer`.
///
/// `de::accessor(item)` is a helper function that avoids having to write 
/// the template type.
///
/// @tparam T Type of object to deserialize
template <class T>
class Accessor : public de::Deserialize {
public:
    /// @brief de::Accessor Constructor
    /// @param output Reference of variable to deserialize into
    explicit Accessor(T & output) : output(output) {
    }

    /// @brief Invoke `de::deserialize<T>()`
    /// @param deserializer Deserializer to extract from
    void deserialize(Deserializer & deserializer) override {
        de::deserialize(deserializer, output);
    }

    /// @brief Get the traits of type T
    /// @return `TypeTraits::of<T>()`
    serde::TypeTraits traits() const override {
        return serde::TypeTraits::of<T>();
    };

private:
    /// @brief Reference of variable to deserialize into
    T & output;
};

/// @brief Helper function to construct `de::Accessor<T>`
/// Avoids having to write the template type during construction.
/// @tparam T Type of object to deserialize
/// @param output Reference of variable to deserialize into
/// @return `de::Accessor<T>`
template <class T>
Accessor<T> accessor(T & output) {
    return Accessor<T>(output);
}


}  // namespace de
}  // namespace kingw
