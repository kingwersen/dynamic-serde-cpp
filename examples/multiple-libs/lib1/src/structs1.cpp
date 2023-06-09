#include "structs1.hpp"

#include "kingw/serde/derive.hpp"

DERIVE_SERDE(MyStruct,
    ("value1", &Self::value1)
    ("value2", &Self::value2)
    ("s",      &Self::s));
