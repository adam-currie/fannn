#pragma once

#include <string>
#include <functional>
#include <vector>

const std::string NAMED_INPUT_PREFIX = "input:";
const std::string NAMED_FUNCTION_PREFIX = "func:";

template <typename T>
std::function<T()> parseUserExp(
    std::string expression, 
    std::function<std::function<T()>(std::string)> getNamedInput,
    std::function<std::function<T(T)>(std::string)> getNamedFunc);

template <typename T>
std::function<void()> getGovernor(
    std::function<T()> getter, 
    std::vector<std::function<void(T)>> setters);
