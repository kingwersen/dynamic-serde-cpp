#pragma once

#include "kingw/ser/derive.hpp"
#include "kingw/de/derive.hpp"


#define DERIVE_SERDE(StructName, ...) \
DERIVE_SERIALIZE(StructName, __VA_ARGS__); \
DERIVE_DESERIALIZE(StructName, __VA_ARGS__);
