#pragma once

# include <vector>
# include <sstream>
# include <string>
# include <ctime>
# include <unistd.h>

namespace ft{
    std::vector<std::string> split(const std::string &s, char delim);
    std::string ltrim(const std::string &s);
    std::string rtrim(const std::string &s);
    std::string trim(const std::string &s);

    // random string generator
    std::string RandString(int len);
    // Hexadecimal number to Decimel
    size_t     HexToDec(std::string nb);
}