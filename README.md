# dynamic-serde-cpp

C++ Serialization/Deserialization library that uses dynamic dispatch to decouple code compilation into separate archives. NOT header-only by design.

This library aims to solve compilation speed and size challenges when working with a large number of data types and multiple serialization adapters at a small cost to execution speed.

In the following example, `structs.hpp` does not include serialization. The serialization logic is contained entirely within `structs.cpp` and independent of the serialization adapter `JsonSerializer`. If the user attempts to serialize an object that does not have serialize implemented, then it will throw an exception at runtime.
```
// structs.hpp

struct ExampleStruct {
    std::string name;
    int value;
    std::vector<int> items;
};
```
```
// structs.cpp

#include <kingw/serde/serialize.hpp>
using namespace kingw;

template <>
void kingw::ser::serialize<ExampleStruct>(ser::Serializer & serializer, const ExampleStruct & value) {
    auto structs = serializer.serialize_struct();
    structs.serialize_field(ser::accessor(value.name), "name");
    structs.serialize_field(ser::accessor(value.value), "value");
    structs.serialize_field(ser::accessor(value.items), "items");
}
```
```
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

    // Prints: {"name":"Hello, World!","value":5,"items":[1,2,3]}
    std::cout << kingw::JsonSerializer::to_string(data) << "\n";
    
    return 0;
}
```
