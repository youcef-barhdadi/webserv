//
//
//


# pragma once

# include "../RequestHeaderParser/RequestHeader.hpp"


class Response{
public:
    Response(RequestHeader const &req);
    ~Response(void);

    void                prepare_headers(void);
    std::vector<char>   read_raw_file(void);

private:
    RequestHeader   _req;
    std::string     _headers;

    // doesn't handle transfer-encoding = chunked
    std::string     _body;
};