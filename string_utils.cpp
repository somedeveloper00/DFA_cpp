#include "string_utils.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>

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

// The main recursive method
// to print all possible
// strings of length k
void printAllKLengthRec(std::vector<std::string>& result, std::string prefix, char set[], int n, int k)
{
     
    // Base case: k is 0,
    // print prefix
    if (k == 0) {
        // std::cout << (prefix) << std::endl;
        result.push_back(prefix);
        return;
    }
 
    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; i++) {
        std::string newPrefix;
         
        // Next character of input added
        newPrefix = prefix + set[i];
         
        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(result, newPrefix, set, n, k - 1);
    }
 
}

std::vector<std::string> string_utils::all_possible_strings(std::string input, int length) {
    std::vector<std::string> result;
    int len = input.length();
    char* set = new char[input.length()];
    strcpy(set, input.c_str());
    printAllKLengthRec(result, std::string(), set, input.length(), length);
    delete[] set;
    return result;
}