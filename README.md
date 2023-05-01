# dynamic-serde-cpp

C++ Serialization/Deserialization library based on the Rust [Serde](https://serde.rs/) API but instead uses dynamic dispatch to decouple code compilation into separate archives/libraries for reuse.

This library aims to solve compilation speed and size challenges when working with a large number of fixed data types and/or multiple serialization adapters. It is by design NOT header-only and is expected to be slower at runtime than Rust's Serde or other header-only implementations as a result of the dynamic dispatching. 

You may find this library useful if you work in an organization that uses a large, centralized data model. The data model can be compiled into an archive and handed out to the team. This may become obsolete when C++ Modules become widespread.

I decided to model the API off of Rust Serde because Serde has a clever data-model-vs-data-format separation, is well-documented, and is widely used (in Rust).

See [serdepp](https://github.com/injae/serdepp/tree/main) for a faster, header-only alternative with a similar API.


## Example

In the following example, `structs.hpp` does not include serialization. The serialization logic is contained entirely within `structs.cpp` and is independent of the serialization adapter `JsonSerializer`.

If `serialize()` is not implemented for any type being serialized, then it will cause a linker error in the unit where the serialization is performed.
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

#include "structs.hpp"

#include <kingw/serde/serialize.hpp>
#include <kingw/serde/accessor.hpp>
using namespace kingw;

template <>
void kingw::ser::serialize<ExampleStruct>(ser::Serializer & serializer, const ExampleStruct & value) {
    serializer.serialize_struct()
        .with_field(ser::accessor(value.name), "name")
        .with_field(ser::accessor(value.value), "value")
        .with_field(ser::accessor(value.items), "items");
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
