//
//  RequestHeader.cpp   7 feb 2020  ztaouil
//

#include "RequestHeader.hpp"

RequestHeader::RequestHeader(std::string &request)
: _full_request(request), _method(), _path(), _protocol_version(0), _raw_body()
{
    Parse();
    ParseQueryParams();
    ParseVerify();
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

    if (firstline.size() != 3)
        throw RequestError();

    _method = firstline[0];
    _path = firstline[1];
    _protocol_version = stof(ft::split(firstline[2], '/')[1]);

    while (std::getline(ss, buffer))
    {
        if (buffer.find(':') == std::string::npos)
            break;
        std::vector<std::string> myvec = ft::split(buffer, ':');
        _headers.insert(std::make_pair(ft::trim(myvec[0]), ft::trim(myvec[1])));
    }

    while (std::getline(ss, buffer))
    {
        _raw_body += buffer + "\n";
    }
    if (!_raw_body.empty())
        _raw_body.erase(_raw_body.end() - 1);
}

void    RequestHeader::ParseVerify(void)
{
    std::string methods[3] = {"GET", "POST", "DELETE"};
    int found = 0;

    for(int i = 0; i < sizeof(methods)/sizeof(std::string); i++)
        if (_method == methods[i])
            found = 1;
    
    if (!found)
        throw RequestError();
}

void    RequestHeader::ParseQueryParams(void)
{
    if (_path.find('?') != std::string::npos)
    {
        std::vector<std::string> myvec1 = ft::split(_path, '?');
        _path = myvec1[0];
        std::vector<std::string> myvec2 = ft::split(myvec1[1], '&');
        int n = std::count(myvec1[1].begin(), myvec1[1].end(), '&');
        if (myvec2.size() != n + 1){
            throw BadRequest();
        }
        for(int i = 0; i < myvec2.size(); i++)
        {
            std::vector<std::string> myvec3 = ft::split(myvec2[i], '=');
            _query_params.insert(std::make_pair(myvec3[0], myvec3[1])); 
        }
    }
}

bool    RequestHeader::QueryParamsEmpty(void)
{
    if (!_query_params.size())
        return true;
    return false;
}

bool    RequestHeader::BodyEmpty(void)
{
    if (_raw_body.empty())
        return true;
    return false;
}
void   RequestHeader::debug_query_params(void)
{
    std::cout << std::string(20, '-') << "+QUERYPARAMS+" << std::string(20, '-') << std::endl;
    std::map<std::string, std::string>::iterator it = _query_params.begin();
    for(; it != _query_params.end(); it++){
        std::cout << it->first << ": " << it->second << std::endl;
    }
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
    std::cout << std::string(20,'*') << "raw_body" << std::string(20,'*') << std::endl;
    return _raw_body;
}

float RequestHeader::get_version(void)
{
    return _protocol_version;
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