//
//  RequestHeader.hpp   7 feb 2020  ztaouil
//
#pragma once

#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <exception>
#include <algorithm>
#include "../utils/utils.hpp"

class RequestHeader{
    public:
        RequestHeader(std::string &request);
        ~RequestHeader(void);

        class   RequestError : public std::exception{
            public:
                virtual const char *what(void) const throw(){
                    return "RequestError";
                }
        };
        class   BadRequest : public std::exception{
            public:
                virtual const char *what(void) const throw(){
                    return "BadRequest";
                }
        };

        void        Parse(void);
        void        ParseVerify(void);
        void        ParseQueryParams(void);

        bool        QueryParamsEmpty(void);
        bool        BodyEmpty(void);

        std::string get_method(void);
        std::string get_path(void);
        float       get_version(void);
        void        debug_headers(void);
        void        debug_query_params(void);
        std::string get_raw_body(void);

        void        get_full_request(void);
    private:
        std::string         _full_request;
        std::string         _method;
        std::string         _path;
        float               _protocol_version;
        std::map<std::string, std::string> _headers;
        std::map<std::string, std::string> _query_params;
        std::string         _raw_body;
};