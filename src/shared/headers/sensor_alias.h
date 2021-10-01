#pragma once
#include <string>

typedef struct SensorAlias {
    std::string alias, id;
    bool operator==(const SensorAlias&) const = default;
} SensorAlias;