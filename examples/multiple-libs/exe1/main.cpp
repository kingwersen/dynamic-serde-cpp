#include <iostream>

#include "kingw/serde_sprintf.hpp"
#include "kingw/serde_json.hpp"
#include "kingw/serde_xml.hpp"
using namespace kingw;

#include "structs1.hpp"


int main() {
    MyStruct data{ 1, 500.0, { { "Hello", "World!" } } };

    //
    // JsonSerializer (high level string manipulation)
    //
    auto str = serde_json::to_string(data);
    std::cout << str << "\n";
    std::cout << serde_xml::to_string(data) << "\n";

    MyStruct data2 = serde_json::from_string<MyStruct>(str);
    std::cout << data2.value2 << "\n";

    auto data3 = serde_json::from_string
        <std::map<std::string, std::string>>("{\"Hello\":\"World!\"}");
    for (auto & kvp : data3) {
        std::cout << kvp.first << ":" << kvp.second << ", ";
    }
    std::cout << "\n\n";

    //
    // SPrintfSerializer (low level string manipulation)
    //
    char buffer[128] = {};
    char* end = serde_sprintf::to_buffer(data, std::begin(buffer), std::end(buffer));

    std::cout << "SPrintfSerializer output: ";
    for (char* iter = buffer; iter < end; ++iter) {
        std::cout << (*iter != '\0' ? *iter : ',');
    }
    std::cout << "\n";

    MyStruct data4;
    serde_sprintf::from_string(data4, serde::string_view(buffer, sizeof(buffer)));
    std::cout << "SPrintfDeserializer output: " << data4.value2 << "\n";

    return 0;
}
