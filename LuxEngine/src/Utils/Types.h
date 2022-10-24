#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <array>

namespace Lux
{

template<typename T>
using List = std::vector<T>;

using String = std::string;

template<typename T, int size>
using Array = std::array<T, size>;

}