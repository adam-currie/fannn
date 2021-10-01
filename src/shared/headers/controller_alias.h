#pragma once
#include <string>

typedef struct ControllerAlias {
    std::string alias, id;
    bool operator==(const ControllerAlias&) const = default;
} ControllerAlias;