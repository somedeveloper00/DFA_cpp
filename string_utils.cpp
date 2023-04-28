#include "string_utils.hpp"

std::vector<std::string> string_utils::split(std::string str, std::string delimiter) {
    std::vector<std::string> r;
    while(true) {
        int p = str.find(delimiter);
        if (p == std::string::npos) {
            r.push_back(str);
            break;
        }
        r.push_back(str.substr(0, p));
        str.erase(0, p+delimiter.length());
    }
    return r;
}