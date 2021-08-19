#pragma once

#include <string>
#include <functional>
#include <vector>

const std::string NAMED_INPUT_PREFIX = "input:";
const std::string NAMED_FUNCTION_PREFIX = "func:";

std::function<int()> parseUserExp(
    std::string expression, 
    std::function<std::function<int()>(std::string)> getNamedInput,
    std::function<std::function<int(int)>(std::string)> getNamedFunc);

template<typename T>
std::function<void()> GetGovernor(
    std::function<T()> getter, 
    std::vector<std::function<void(T)>> setters);