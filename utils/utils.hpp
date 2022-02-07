#pragma once

# include <vector>
# include <sstream>
# include <string>

namespace ft{
    std::vector<std::string> split(const std::string &s, char delim);
    std::string ltrim(const std::string &s);
    std::string rtrim(const std::string &s);
    std::string trim(const std::string &s);
}