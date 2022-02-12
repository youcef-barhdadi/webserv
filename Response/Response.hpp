//
//
//


# pragma once
# include <fcntl.h>
# include <stdlib.h>
# include <memory>
# include "../RequestHeaderParser/RequestHeader.hpp"
# include "MimeTypes/MimeTypes.h"


class Response{
public:
	Response(RequestHeader const &req);
	~Response(void);

	void				prepare_headers(void);

	void				read_raw_file(void);
	std::string			extract_extension(void);

	std::string			build_response(void);


private:
	RequestHeader   _req;
	std::string _status_line;
	std::map<std::string, std::string>  _headers;

	// doesn't handle transfer-encoding = chunked
	std::string  _body;

	int			_file_flag;
};