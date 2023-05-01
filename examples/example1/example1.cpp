#include <iostream>

#include "kingw/json_serializer.hpp"
#include "kingw/json_deserializer.hpp"
#include "kingw/xml_serializer.hpp"

#include "structs1.hpp"


int main() {
    MyStruct data{ 1, 500.0, { { "Hello", "World!" } } };
    std::cout << kingw::JsonSerializer::to_string(data) << "\n";
    std::cout << kingw::XmlSerializer::to_string(data) << "\n";

    auto data2 =  kingw::JsonDeserializer::from_string
        <std::vector<int>>("[1,2,3,4,5]");
    for (auto & item : data2) {
        std::cout << item << ",";
    }
    std::cout << "\n";

    auto data3 = kingw::JsonDeserializer::from_string
        <std::map<std::string, std::string>>("{\"Hello\":\"World!\"}");
    for (auto & kvp : data3) {
        std::cout << kvp.first << ":" << kvp.second << ", ";
    }
    std::cout << "\n";

    return 0;
}
