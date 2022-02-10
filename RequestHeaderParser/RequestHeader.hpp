//
//  RequestHeader.hpp   7 feb 2020  ztaouil
//
#pragma once

#include <ctime>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <exception>
#include <algorithm>
#include <fstream>
#include "../utils/utils.hpp"

class RequestHeader{
    public:
        RequestHeader(void);
        RequestHeader(RequestHeader const &rhs);
        ~RequestHeader(void);
        RequestHeader &operator= (RequestHeader const &rhs);

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

        void        Append(std::string &Message);

        bool        HeadersFinished(void);
        bool        BodyFinished(void);
        bool        IsFinished(void);

        void        ParseHeaders(void);

        void        ParseVerify(void);
        void        ParseQueryParams(void);

        bool        QueryParamsEmpty(void);

		void		req_get(void);

        std::string get_method(void);
        std::string get_path(void);
        float       get_version(void);
        void        debug_headers(void);
        void        debug_query_params(void);

        void        get_buffer(void);
    private:
        std::string         _buffer;
        std::string         _method;
        std::string         _path;
        float               _protocol_version;
        std::map<std::string, std::string> _headers;
        std::map<std::string, std::string> _query_params;
        std::string         _body_filename;
        size_t              _body_size;
        bool                _isFinished;
        bool                _isHeaderParsed;

		

        //
        int          _debug;
};