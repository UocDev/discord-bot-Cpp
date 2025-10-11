#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

inline std::string read_token_file(const std::string &path = "token.txt") {
    std::ifstream in(path);
    if (!in.is_open()) return std::string{};
    std::string s;
    std::getline(in, s);
    // trim whitespace
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
    return s;
}
