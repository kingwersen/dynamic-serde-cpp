# dynamic-serde-cpp (wip)

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

`structs.cpp` contains the implementation of Serde for `ExampleStruct`. Here, we use the `DERIVE_SERDE()` macro to simplify implementation. The first parameter of the macro refers to the name of the class or struct, and the following parameters are a list of name:expression pairs that identify the fields. The name is a unique identifier, and the expression should return a reference to the field to read/write, such as `object.foo` or `object.getFoo()` (always use `object` to refer to the object). If accessing your data fields is nontrivial, then you may need to derive [serialize](include/kingw/ser/derive.hpp) and/or [deserialize](include/kingw/de/derive.hpp) manually.

```c++
// structs.cpp

#include "structs.hpp"
#include <kingw/serde/derive.hpp>

// Implement serialize<>() and deserialize<>() for this class.
// Member variables are de/serialized based on each of their types automatically,
// provided their types also have Serde implemented.
DERIVE_SERDE(ExampleStruct,
    name,   object.name,
    value,  object.value,
    items,  object.items);
```

`main.cpp` performs the serialization of the structure into JSON. Since the Serde implementation for `ExampleStruct` was defined in the C++ source file and not the header file, `main.cpp` does not know how to serialize the structure. This information is instead provided by the linker after compilation. If Serde is not derived for the structure in any source file or linked library, then a linker error will be raised and the build will fail.
```c++
// main.cpp

#include <iostream>
#include <kingw/json_serializer.hpp>
#include "structs.hpp"

int main() {
    ExampleStruct data {
        "Hello, World!",
        5,
        { 1, 2, 3 }
    };

    // Prints: {"items":[1,2,3],"name":"Hello, World!","value":5}
    std::cout << kingw::JsonSerializer::to_string(data) << "\n";
    
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
