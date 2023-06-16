# dynamic-serde-cpp (discontinued)

![CI Status](https://github.com/kingwersen/dynamic-serde-cpp/actions/workflows/linux.yml/badge.svg)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=kingwersen_dynamic-serde-cpp&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=kingwersen_dynamic-serde-cpp)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=kingwersen_dynamic-serde-cpp&metric=coverage)](https://sonarcloud.io/summary/new_code?id=kingwersen_dynamic-serde-cpp)

Another C++ port of the Rust (language) [Serde](https://serde.rs/) serialization and deserialization framework.

Unlike other ports, **dynamic-serde-cpp** deliberately uses dynamic dispatch to decouple code compilation into separate archives/libraries for reuse and to improve compile times.


## Overview

The Serde framework decouples serialization/deserialization into two independent components: the **data format** (like JSON or XML) versus the **data model** (your classes and structs). Each of these components can be implemented independently of one another and are fully interoperable. A developer can implement Serde for their class *once* and serialize/deserialize it using every format that also has a Serde implementation.

This repository contains:
- The core Serde framework
- `DERIVE_SERDE(YourClass, ...)` macros for easy integration
- A Serde-compatible JSON serializer/deserializer for example (implemented using [nlohmann json](https://github.com/nlohmann/json))


## Design

This library aims to solve compilation speed and size challenges when working with a large number of fixed data types by allowing Serde implementations to be compiled into libraries. Data model and data format implementations can be defined entirely in .cpp source files and linked together, instead of being defined in header files and bloating every compilation unit. The consequence of this decision is that the compiler cannot optimize as well, resulting in slower execution speed compared to Rust's Serde or other C++ header-only implementations.

You may find this library useful if you use a large, centralized data model. The data model can be compiled into an archive and passed around. By comparison, header-only implementations usually cannot be compiled into libraries.

See [serdepp](https://github.com/injae/serdepp/tree/main) for a faster, header-only alternative.


## :warning: Discontinuation Notice :warning:

During development I found some significant logic gaps in the C++ compiler/linker that make this Serde implementation impractical. There are certain use cases that are impossible without taxing the end user.

This implementation is incomplete and I do not plan to finish it.

The goal was to compile Serde definitions into libraries which could be linked against. If you aren't using template types, this works great. However, template types get messy. Template types don't get compiled into libraries at all and must be included from a header file in every source file in which they're used. This normally wouldn't be a problem, but the Serde definitions can create circular dependencies, and since the implementations can't be compiled into a library, there's no practical way to organize the definitions that prevents circular dependency errors. Technically, the user can declare all their template Serde implementations in advance, but this is totally impractical.

The following example demonstrates this issue. Notice that there's a linker error no matter which way I choose to order the serialize() definitions:
```c++
template <class T> struct A { T value; };
template <class T> struct B { T value; };

// Define serialize() for A, then B
template <class T>
ser::serialize(ser::Serializer & serializer, const A<T> & input) {
    ser::serialize(serializer, input.value);
}
template <class T>
ser::serialize(ser::Serializer & serializer, const B<T> & input) {
    ser::serialize(serializer, input.value);
}

ser::serialize(serializer, A<B<int>>{ { 5 } });  // Linker Error - serialize<B<int>>()
ser::serialize(serializer, B<A<int>>{ { 5 } });  // OK
```
```c++
template <class T> struct A { T value; };
template <class T> struct B { T value; };

// Define serialize() for B, then A
template <class T>
ser::serialize(ser::Serializer & serializer, const B<T> & input) {
    ser::serialize(serializer, input.value);
}
template <class T>
ser::serialize(ser::Serializer & serializer, const A<T> & input) {
    ser::serialize(serializer, input.value);
}

ser::serialize(serializer, A<B<int>>{ { 5 } });  // OK
ser::serialize(serializer, B<A<int>>{ { 5 } });  // Linker Error - serialize<A<int>>()
```
This is because template functions do not support partial specialization. I cannot do ``template <class T> ser::serialize<A<T>>(const A<T> & value) { ... }``, only ``template <class T> ser::serialize(const A<T> & value) { ... }``. The latter is a completely different function signature than the original, and since it wasn't declared before it was referenced in another serialize() implementation, the original, non-template implementation ``template <> serialize<A<int>>()`` is used instead. This is non-template implementation is never defined and is what throws the linker error.

Interestingly, I can actually resolve this issue if I replace ``serialize<T>()`` with ``Serde<T>::serialize()`` (a template struct w/ a static function). This appears to cause the C++ compiler to defer template resolution until the template is used instead of when the template is defined (?). However, the struct functions are never written to libraries. As a result, this is a 100% header-only solution and is not usable in our library-based solution. All Serde implementations, including integral types like integers, must be included everywhere:
```c++
template <class T> struct A { T value; };
template <class T> struct B { T value; };

// Define serialize() for A, then B
template <class T>
struct Serde<A<T>> {
    static void serialize(ser::Serializer & serializer, const A<T> & input) {
        Serde<T>::serialize(serializer, input.value);
    }
};
template <class T>
struct Serde<B<T>> {
    static void serialize(ser::Serializer & serializer, const B<T> & input) {
        Serde<T>::serialize(serializer, input.value);
    }
};

// Order no longer matters
Serde<A<B<int>>>::serialize(serializer, A<B<int>>{ { 5 } });  // OK
Serde<B<A<int>>>::serialize(serializer, B<A<int>>{ { 5 } });  // OK
```

There are other frustrating problems that revolve around include ordering and are unique to this library-based implementation:
```c++
// Must include stdvector.hpp first or from_string() throws a linker error.
#include "kingw/serde/templates/stdvector.hpp"
#include "kingw/serde_json.hpp"
serde_json::from_string(std::vector<int>{ 1, 2, 3 });
```

The apparent solution is to use a header only implementation. Injae already has one here: [serdepp](https://github.com/injae/serdepp/tree/main).


## Basic Usage

The following example demonstrates implementing Serde for a custom structure and serializing it in JSON.

`structs.hpp` defines the structure itself. Notice that it contains no references to Serde at all.
```c++
// structs.hpp

struct ExampleStruct {
    std::string name;
    int value;
    std::vector<int> items;
};
```

`structs.cpp` contains the implementation of Serde for `ExampleStruct`. Here, we use the `DERIVE_SERDE()` macro to simplify implementation. The first parameter of the macro refers to the name of the class or struct, and the following parameters are a list of ("name", expression) pairs that identify the fields. The name is a unique identifier, and the expression should return a reference to the field to read/write. If accessing your data fields is nontrivial, then you may need to derive serialize and/or deserialize manually.

```c++
// structs.cpp

#include "structs.hpp"
#include <kingw/serde/derive.hpp>

// Implement serialize<>() and deserialize<>() for this class.
// Member variables are de/serialized based on each of their types automatically,
// provided their types also have Serde implemented.
DERIVE_SERDE(ExampleStruct,
    ("name",    &Self::name)
    ("value",   &Self::value)
    ("items",   &Self::items));
```

`main.cpp` performs the serialization of the structure into JSON. Since the Serde implementation for `ExampleStruct` was defined in the C++ source file and not the header file, `main.cpp` does not know how to serialize the structure. This information is instead provided by the linker after compilation. If Serde is not derived for the structure in any source file or linked library, then a linker error will be raised and the build will fail.
```c++
// main.cpp

#include <iostream>
#include <kingw/serde_json.hpp>
#include "structs.hpp"
using namespace kingw;

int main() {
    ExampleStruct data {
        "Hello, World!",
        5,
        { 1, 2, 3 }
    };

    // Prints: {"items":[1,2,3],"name":"Hello, World!","value":5}
    std::cout << serde_json::to_string(data) << "\n";
    
    return 0;
}
```

## Include in your Project
### CMake
This project comes with a CMake build configuration. You can include it in your project using FetchContent:

```cmake
# Download this repository
include(FetchContent)
FetchContent_Declare(kingw_dynamic_serde
    GIT_REPOSITORY  "https://github.com/kingwersen/dynamic-serde-cpp.git"
    GIT_TAG         "main")
FetchContent_MakeAvailable(kingw_dynamic_serde)

# Link against your project
add_library(your_library)
target_link_libraries(your_library
    PUBLIC
        kingw::dynamic_serde        # Core framework
        kingw::dynamic_serde_json)  # JSON implementation
```

CMake flags:
- `-D KINGW_SERDE_BUILD_TESTS=ON`: Build unit tests
- `-D KINGW_SERDE_BUILD_EXAMPLES=ON`: Build examples
