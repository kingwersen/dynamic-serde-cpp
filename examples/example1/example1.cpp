#include <iostream>

#include "kingw/json_serializer.hpp"
#include "kingw/xml_serializer.hpp"

#include "structs1.hpp"


int main() {
    MyStruct data{ 1, 500.0, { { "Hello", "World!" } } };

    // Ex1: Provide std::ostream to constructor
    kingw::JsonSerializer serializer(std::cout, true);
    serializer.serialize(data);
    std::cout << "\n";

    // Ex2: Simple to_string()
    std::cout << kingw::XmlSerializer::to_string(data) << "\n";
    return 0;
}
