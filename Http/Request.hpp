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
#include "../Utilities/Utilities.hpp"

#include "../Server/Server.hpp"




class Request{
	public:
		Request(void);
		Request(Request const &rhs);
		~Request(void);
		Request &operator= (Request const &rhs);

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
		bool        IsFinished(void) const;
		void        ParseHeaders(void);

		void        VerifyRequest(void);
		void        ParseQueryParams(void);

		bool        QueryParamsEmpty(void);

		std::string get_method(void) const;
		std::string get_path(void) const;
		float       get_version(void);
		void        debug_headers(void);
		void        debug_query_params(void);
		void		set_path(std::string path);
		std::string	get_body_filename(void);

		void        get_buffer(void);

		void        set_server(Server *server);


		bool        HasHeader(std::string header,  std::string value);

		Server       *_server;
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
