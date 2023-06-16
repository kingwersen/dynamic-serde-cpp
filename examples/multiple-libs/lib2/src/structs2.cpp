#include "structs2.hpp"

#include "kingw/serde/derive.hpp"
#include "kingw/serde/templates/stdvector.hpp"

DERIVE_SERDE(MyStruct2,
    ("contents", &Self::contents));
