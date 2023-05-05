#include "structs2.hpp"

#include "kingw/serde/derive.hpp"

DERIVE_SERDE(MyStruct2,
    contents, object.contents);
