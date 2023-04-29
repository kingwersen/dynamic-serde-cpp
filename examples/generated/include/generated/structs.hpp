#pragma once

#include <vector>
#include <string>


struct MyStruct2 {
    std::vector<std::string> contents;
};

struct MyStruct {
    int value1;
    double value2;
    MyStruct2 s;
};

void foo();
