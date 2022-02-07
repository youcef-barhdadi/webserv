//
//  RequestHeader.cpp   7 feb 2020  ztaouil
//

#include "RequestHeader.hpp"

RequestHeader::RequestHeader(std::string &request)
: _full_request(request), _method(), _path(), _protocol_version(0), _raw_body(), _error(0)
{
    Parse();
}

RequestHeader::~RequestHeader(void)
{

}

void    RequestHeader::Parse(void)
{
    std::stringstream   ss(_full_request);

    std::string buffer;
    std::getline(ss, buffer);
    std::vector<std::string> firstline = ft::split(buffer, ' ');

    if (firstline.size() == 3){
        _method = firstline[0];
        _path = firstline[1];
        _protocol_version = stof(ft::split(firstline[2], '/')[1]);
        _error = 1;
    }

    while (std::getline(ss, buffer))
    {
        if (buffer.find(':') == std::string::npos)
            break;
        std::vector<std::string> myvec = ft::split(buffer, ':');
        _headers.insert(std::make_pair(ft::trim(myvec[0]), ft::trim(myvec[1])));
    }

    while (std::getline(ss, buffer))
    {
        std::cout << "##" << buffer << std::endl;
        _raw_body += buffer + "\n";
        
        // std::cout << buffer << std::endl;
    }
    _raw_body.erase(_raw_body.end() - 1);
}

std::string RequestHeader::get_method(void)
{
    return _method;
}

std::string RequestHeader::get_path(void)
{
    return _path;
}

std::string RequestHeader::get_raw_body(void)
{
    std::cout << std::string(42,'*') << "raw_body" << std::string(42,'*') << std::endl;
    return _raw_body;
}

float RequestHeader::get_version(void)
{
    return _protocol_version;
}

int     RequestHeader::get_error(void)
{
    return _error;
}

void    RequestHeader::debug_headers(void)
{
    std::map<std::string, std::string>::iterator it = _headers.begin();

    for(; it != _headers.end(); it++){
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void    RequestHeader::get_full_request(void)
{
    std::cout << _full_request << std::endl;
}