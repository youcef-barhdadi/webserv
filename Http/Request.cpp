//
//  Request.cpp   7 feb 2020  ztaouil
//

#include "Request.hpp"

Request::Request(void)
: _buffer(), _method(), _path(), _protocol_version(0), _body_filename(), _body_size(0)
, _isFinished(false), _isHeaderParsed(false), _bad_status(0), ofs_open(false)
{
	_my_server = NULL;
}

Request::~Request(void)
{

}

Request::Request(Request const &rhs)
: _buffer(), _method(), _path(), _protocol_version(0), _body_filename(), _body_size(0)
, _isFinished(rhs.IsFinished()), _isHeaderParsed(false)
{
	// std::cout << "Request::Request(Request const &rhs)" << std::endl;
	*this = rhs;

}

Request   &Request::operator=(Request const &rhs)
{
	// std::cout << "Request::operator=" << std::endl;
	if (this != &rhs){
		_buffer = rhs._buffer;
		_method = rhs._method;
		_path = rhs._path;
		_protocol_version = rhs._protocol_version;
		_headers = rhs._headers;
		_query_params = rhs._query_params;
		_body_filename = rhs._body_filename;
		_body_size = rhs._body_size;
		_isFinished = rhs._isFinished;
		_isHeaderParsed = rhs._isHeaderParsed;
	}

	return *this;
}

void    Request::Append(std::string &Message)
{
// binary go brrrr
	std::vector<char> myvec;
	for (size_t i = 0; i < Message.size(); i++){
		// std::cerr << (int)Message[i] << " ";
		myvec.push_back(Message[i]);
	}
	// std::cerr << std::endl;
	_buffer.insert(_buffer.end(), myvec.begin(), myvec.end());

	// _buffer += Message;

 
	if (_isHeaderParsed == false && HeadersFinished()){
		ParseHeaders();
		ParseQueryParams();
		_isHeaderParsed = true;
		// find_server();
		find_location();
		// std::cout << _my_server->get_host() << std::endl;
		// std::cout << _mylocation << std::endl;
	}

	if (_isFinished)
		return ;

	if (_isHeaderParsed && _headers.find("Content-Length") != _headers.end() && std::stoi(_headers["Content-Length"]) > 0)
	{
		if (_body_filename.size() == 0)
			_body_filename = RandString(30);
		if (ofs_open == false && _method == "POST"){
			std::cerr << "ofs opened" << std::endl;
			f_fd = open(_body_filename.c_str(), O_CREAT|O_APPEND|O_RDWR,  S_IRUSR|S_IWUSR);
			fcntl(f_fd, F_SETFL, O_NONBLOCK);
			FD_ZERO(&f_fd_set);
			FD_SET(f_fd, &f_fd_set);
			// ofs.open(_body_filename, std::ofstream::out);
			ofs_open = true;
		}
		if (_headers["Transfer-Encoding"] == "chunked")
		{
			// std::cerr << "Transfer-Encoding: chunked" << std::endl;
			std::stringstream ss(_buffer);
			std::string buff;

			// postman dlkhra
			// std::getline(ss, buff);			

			while (true)
			{
				std::getline(ss, buff);
				size_t n = HexToDec(buff);

				// std::cerr << "n = " << n << std::endl;

				if (!n)
					break;

				for(size_t i = 0; i < n; i++){
					char c = ss.get();
					// ofs << c;
					select(f_fd + 1, NULL, &f_fd_set, NULL, NULL);
					write(f_fd, &c, 1);
					// // std::cerr << "[" << (int)c << "]";
				}
				// // std::cerr << std::endl;
				ss.get();
			}

		}else{
			select(f_fd + 1, NULL, &f_fd_set, NULL, NULL);
			// ofs << _buffer;
			for (size_t i = 0; i < _buffer.size(); i++)
				write(f_fd, &_buffer[i], 1);
			_body_size += _buffer.size();
			// std::cerr << "#" << _body_size << " | " << _headers["Content-Length"] << std::endl;
			if (_body_size+1 >= static_cast<size_t>(std::stoi(_headers["Content-Length"])))
				_isFinished = 1;
			if (_body_size > _my_server->get_client_body_size() * 1048576)
			{
				_isFinished = 1;
				_bad_status = 413;
			}
			// std::cout << "==>" << _body_size << std::endl;
		}
		_buffer.clear();
		if (_isFinished && ofs_open){
			// ofs.close();
			close(f_fd);
			ofs_open = false;
			std::cerr << "ofs closed" << std::endl;
		}
	}
	else if (_headers["Transfer-Encoding"] != "chunked" && BodyFinished())
		_isFinished = true;

	if (_isFinished)
	{
		VerifyRequest();
		// ofs.close();
		if (ofs_open){
			close(f_fd);
			ofs_open = false;
		}
		std::cerr << "ofs closed" << std::endl;
	}

	// std::cerr << "is finished = " << _isFinished << std::endl;
	// std::cerr << "body size "  << _body_size << std::endl;
}

bool    Request::HeadersFinished(void)
{
	std::stringstream ss(_buffer);
	std::string buffer;
	bool cond(false);

	while (std::getline(ss, buffer))
	{
		if (buffer == "\r"){
			cond = true;
			break;
		}
	}
	return cond;
}

bool    Request::BodyFinished(void)
{
	size_t content_length = 0;
	if (_headers.find("Content-Length") != _headers.end())
		content_length = std::stoi(_headers["Content-Length"]);

	return content_length <= _body_size;
}

bool     Request::IsFinished(void) const
{
	return _isFinished;
}

void       Request::ParseHeaders(void)
{
	std::cerr << "Request::Parseheaders" << std::endl;
	std::stringstream   ss(_buffer);
	// // std::cerr << _buffer << std::endl;

	std::string buffer;
	std::getline(ss, buffer);
	std::vector<std::string> firstline = split(buffer, ' ');
//
	if (firstline.size() != 3){
		_bad_status = 400;
		_isFinished = true;
		return ;
	}

	_method = firstline[0];
	_path = firstline[1];
	_protocol_version = stof(split(firstline[2], '/')[1]);

	// // std::cerr << "protocol version = " << _protocol_version << std::endl;
	if (_protocol_version >= 1.2)
	{
		_bad_status = 505;
		_isFinished = true;
		return ;
	}

	while (std::getline(ss, buffer))
	{
		if (buffer == "\r")
			break;
		std::vector<std::string> myvec = split(buffer, ':');
		if (myvec[0].find(' ') != std::string::npos){
			_bad_status = 400;
			_isFinished = true;
			return ;
		}
		if (_headers.find(trim(myvec[0])) != _headers.end())
		{
			_bad_status = 400;
			_isFinished = true;
			return ;
		}
		_headers.insert(std::make_pair(trim(myvec[0]), trim(myvec[1])));
	}

	// 
	find_server();
	std::cerr << "bug hunter big daddy" << std::endl;
	try{
		if (_headers.find("Content-Length") != _headers.end() && (std::stoi(_headers["Content-Length"]) < 0))
		{
			_bad_status = 400;
			_isFinished = true;
			return ;
		}
		if (_headers.find("Content-Length") != _headers.end() && std::stoi(_headers["Content-Length"]) > static_cast<int>(_my_server->get_client_body_size() * 1048576))
		{
			_bad_status = 413;
			_isFinished = true;
			return ;
		}
	}catch(...){
		_bad_status = 413;
		_isFinished = true;
		return ;		
	}

	_buffer.clear();
	while (std::getline(ss, buffer))
	{
		_buffer += buffer + "\n";
	}
	if (!_buffer.empty())
		_buffer.erase(_buffer.end() - 1);
}

void    Request::VerifyRequest(void)
{
	// std::cout << "Request::ParseVerify" << std::endl;
	std::string methods[3] = {"GET", "POST", "DELETE"};
	int found = 0;

	for(int i = 0; i < static_cast<int>(sizeof(methods)/sizeof(std::string)); i++)
		if (_method == methods[i])
			found = 1;
	if (!found){
		_bad_status = 501;
		_isFinished = true;
	}
}

void    Request::ParseQueryParams(void)
{
	if (_path.find('?') != std::string::npos)
	{
		std::vector<std::string> myvec1 = split(_path, '?');
		_path = myvec1[0];
		std::vector<std::string> myvec2 = split(myvec1[1], '&');
		size_t n = std::count(myvec1[1].begin(), myvec1[1].end(), '&');
		if (myvec2.size() != n + 1){
			try{
				throw BadRequest();
			}catch(...){
				_bad_status = 400;
			}
		}
		for(size_t i = 0; i < myvec2.size(); i++)
		{
			std::vector<std::string> myvec3 = split(myvec2[i], '=');
			_query_params.insert(std::make_pair(myvec3[0], myvec3[1]));
		}
	}
}

bool    Request::QueryParamsEmpty(void)
{
	if (!_query_params.size())
		return true;
	return false;
}

void   Request::debug_query_params(void)
{
	std::cout << std::string(20, '-') << "+QUERYPARAMS+" << std::string(20, '-') << std::endl;
	std::map<std::string, std::string>::iterator it = _query_params.begin();
	for(; it != _query_params.end(); it++){
		std::cout << it->first << ": " << it->second << std::endl;
	}
}

std::string Request::get_method(void) const
{
	return _method;
}

std::string Request::get_path(void) const
{
	return _path;
}

float Request::get_version(void)
{
	return _protocol_version;
}

void    Request::debug_headers(void)
{
	std::map<std::string, std::string>::iterator it = _headers.begin();

	for(; it != _headers.end(); it++){
		std::cout << it->first << ": " << it->second << std::endl;
	}
}

void    Request::get_buffer(void)
{
	std::cout << _buffer << std::endl;
}


void		Request::set_path(std::string path)
{
	this->_path = path;
}

std::string	Request::get_body_filename(void)
{
	return _body_filename;
}


void      Request::set_server(std::vector<Server *> server)
{
	this->_server = server;
}

bool        Request::HasHeader(std::string header,  std::string value)
{
	std::map<std::string, std::string>::iterator iter = this->_headers.find(header);


	if (iter == this->_headers.end())
		return false;
	// std::cout << iter->first << " .====={"  << iter->second << "}" << std::endl;
	if (iter->second == value)
		return true;
	return false;
}

std::map<std::string, std::string>  Request::get_query_parnms()
{
	return this->_query_params;
}

std::map<std::string, std::string>  Request::get_query_headers()
{
	return this->_headers;
}

int									Request::get_bad_status(void)
{
	return _bad_status;
}


void				Request::find_location(void)
{
	std::vector<struct location>  loc = _my_server->get_locations();
	std::string req_path = _path;
	int location_index = -1;
	std::string matched_location = "/";

	for(size_t i = 0;  i < loc.size(); i++)
	{
		size_t ret = req_path.find(loc[i].url);
		std::cout << loc[i].url << " " << req_path << std::endl;
		if (ret != std::string::npos && ret == 0 && (req_path[ret + loc[i].url.size()] == '/' || req_path == loc[i].url)){
			if (loc[i].url.length() >= matched_location.length()){
				matched_location = loc[i].url;
				location_index  = i;
			}
		}
	}
	if (location_index != -1)
		this->_mylocation = &_my_server->get_locations()[location_index];
	else
		_mylocation = 0x0;
}

//

void				Request::find_server(void)
{
	for (size_t i = 0; i < _server.size(); i++){
		// std::cerr << _server[i]->get_server_name() << " | " << _headers["Host"] << std::endl;
		if (_server[i]->get_server_name() == _headers["Host"])
		{
			_my_server = _server[i];
			return ;
		}
	}
	_my_server = _server[0];
}



std::string		Request::getHeader(std::string key)
{
	std::map<std::string, std::string>::iterator iter =_headers.find(key);

	if (iter != _headers.end())
		return iter->second;
	return "";
}