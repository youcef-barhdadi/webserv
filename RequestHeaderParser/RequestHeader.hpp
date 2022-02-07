//
//  RequestHeader.hpp   7 feb 2020  ztaouil
//
#pragma once

#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "../utils/utils.hpp"

class RequestHeader{
    public:
        RequestHeader(std::string &request);
        ~RequestHeader(void);

        void    Parse(void);

        std::string get_method(void);
        std::string get_path(void);
        float       get_version(void);
        void        debug_headers(void);
        std::string get_raw_body(void);
        int         get_error(void);

        void    get_full_request(void);
    private:
        std::string         _full_request;
        std::string         _method;
        std::string         _path;
        float               _protocol_version;
        std::map<std::string, std::string> _headers;
        std::string         _raw_body;
        int                 _error;
};