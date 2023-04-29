#pragma once

#include <string>
#include <vector>
#include <set>

namespace string_utils
{
    std::vector<std::string> split(std::string str, std::string delimiter);
    std::vector<std::string> all_possible_strings(std::string input, int length);
} // namespace strUtils
