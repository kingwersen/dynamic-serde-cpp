#include <iostream>

#include "kingw/json_serializer.hpp"
#include "kingw/xml_serializer.hpp"

#include "generated/structs.hpp"


int main() {
    MyStruct data{ 1, 500.0, { { "Hello", "World!" } } };
    foo();  // TODO: Some weird link problem requires this.

    kingw::JsonSerializer serializer(std::cout, true);
    serializer.serialize(data);
    std::cout << "\n";
    std::cout << kingw::XmlSerializer::to_string(data) << "\n";
    return 0;
}
