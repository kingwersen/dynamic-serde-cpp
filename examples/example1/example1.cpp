#include <iostream>

#include "kingw/sprintf_serializer.hpp"
#include "kingw/sprintf_deserializer.hpp"
#include "kingw/json_serializer.hpp"
#include "kingw/json_deserializer.hpp"
#include "kingw/xml_serializer.hpp"

#include "structs1.hpp"


int main() {
    MyStruct data{ 1, 500.0, { { "Hello", "World!" } } };

    //
    // JsonSerializer (high level string manipulation)
    //
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
    std::cout << "\n\n";

    //
    // SPrintfSerializer (low level string manipulation)
    //
    char buffer[128] = {};
    char* end = kingw::SPrintfSerializer::to_buffer(data, buffer, sizeof(buffer));

    std::cout << "SPrintfSerializer output: ";
    for (char* iter = buffer; iter < end; ++iter) {
        std::cout << (*iter != '\0' ? *iter : ',');
    }
    std::cout << "\n";

    MyStruct data4;
    kingw::SPrintfDeserializer::from_buffer(data4, buffer);
    std::cout << "SPrintfDeserializer output: " << data4.value2 << "\n";

    return 0;
}
