//
//
//


# pragma once
# include <fcntl.h>
# include <stdlib.h>
# include <memory>
# include "../Request/Request.hpp"
# include "MimeTypes/MimeTypes.h"
# include "../Server/Config.hpp"


class Response{
public:
	Response(Request const &req, Server const &servers);
	~Response(void);

	void				verifyRequest(void);

	void				prepare_headers(void);

	void				read_raw_file(void);
	std::string			extract_extension(void);

	std::string			build_response(void);

	std::string			Get(void);
	std::string			Post(void);
	std::string			Delete(void);

private:
	Request   _req;
	std::string _status_line;
	std::map<std::string, std::string>  _headers;

	// doesn't handle transfer-encoding = chunked
	std::string  _body;

	int			_file_flag;
	Server const &_server;
};