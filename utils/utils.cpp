#include "utils.hpp"

namespace ft{

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline(ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
 
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

std::string RandString(int len)
{
    srand(time(NULL) * getpid());
    const std::string alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string str;
    str.reserve(len);

    for(int i = 0; i < len; ++i){
        str += alphanum[rand() % (alphanum.size() - 1)];
    }
    return str;
}

size_t     HexToDec(std::string nb)
{
    size_t n;
    std::string tmp;
    std::stringstream ss;

    ss << nb;
    ss >> std::hex >> n;
    
    return n;
}


}