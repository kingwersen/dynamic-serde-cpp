#include <iostream>

#include "kingw/json_serializer.hpp"
#include "kingw/json_deserializer.hpp"
#include "kingw/xml_serializer.hpp"

#include "structs1.hpp"


int main() {
    MyStruct data{ 1, 500.0, { { "Hello", "World!" } } };
    auto str = kingw::JsonSerializer::to_string(data);
    std::cout << str << "\n";
    std::cout << kingw::XmlSerializer::to_string(data) << "\n";

    MyStruct data2 = kingw::JsonDeserializer::from_string<MyStruct>(str);
    std::cout << data2.value2 << "\n";

    auto data3 = kingw::JsonDeserializer::from_string
        <std::map<std::string, std::string>>("{\"Hello\":\"World!\"}");
    for (auto & kvp : data3) {
        std::cout << kvp.first << ":" << kvp.second << ", ";
    }
    std::cout << "\n";

    return 0;
}
