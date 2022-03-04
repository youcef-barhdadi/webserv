#include <fstream>
#include <filesystem>
#include <vector>
#include "../Cgi/Cgi.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include "Response.hpp"
#include "../Utilities/Utilities.hpp"
#include <cstring>
#include <stdio.h>

Response::Response( const Response & rhs)
{
	*this= rhs;
}


Response::Response()
{
	this->_bytes_sent = 0;
	this->_is_finshed = false;
	this->_mylocation = NULL;
	this->_index_file = "";
}
Response::Response(Request * req )
{
	this->_request = req;
	this->_bytes_sent = 0;
	this->_is_finshed = false;
	this->_index_file = "";
}

Response::~Response()
{
	delete this->_request;
}

Response &				Response::operator=( Response const & rhs )
{
	this->_response_vec.insert(this->_response_vec.begin(), rhs._response_vec.begin(), rhs._response_vec.end());// =  rhs.response_vec;
	this->_bytes_sent = rhs._bytes_sent;
	this->_size = rhs._size;
	this->_is_finshed = rhs._is_finshed;
	this->_status = rhs._status;
	this->_request = rhs._request;
	this->_mylocation = rhs._mylocation;
	this->_index_file = rhs._index_file;
	return *this;
}


//
// create_autoindex the root of the location i.e, location /auto /Users/ztaouil/Projects/webserv
//											we will create_autoindex this dir -> /Users/ztaouil/Projects/webserv

void	  Response::create_autoindex(std::string path)
{
	std::cout << "Response::create_autoindex" << std::endl;
	if (path[ path.size() - 1] != '/')
		path.insert( path.end(), '/');

	// 
	std::string absolute_path = get_absolute_path();


	std::string	body = "<html lang='en'>\n<head>\n<title>Document</title>\n</head>\n<body>\n<h1>Index OF "+ path + " </h1>\n<br>\n<table width=\"100%\">\n<hr>";
	std::string  header;
	std::string s = path[0] == '/' ? path.erase(0,1) : path;
	// std::string s = path;
	if (s[s.size()-1] == '/')
		s.erase(s.end() - 1);

	if(isDirectory(absolute_path))
	{
// here	
		std::vector<FileInfo> fileInfoList = getListOfFiles(absolute_path);


		for(size_t i = 0; i < fileInfoList.size(); i++)
		{
			std::string td = "<tr><td width=\"50%\"> <a href=\"/"+ s + "/"+ fileInfoList[i].fileName +"\"> "+fileInfoList[i].fileName + "</a></td>"  ;
			body += td;
			if (fileInfoList[i].fileName == ".." || fileInfoList[i].fileName == ".")
				continue;
			td  = "<td width=\"25%\">"+ fileInfoList[i].date + "</td>"  ;
			body += td;
			if (isDirectory( absolute_path + "/"+ fileInfoList[i].fileName))
				td = "<td width=\"25%\">"+ std::string("-") +"</td></tr>"  ;
			else
				td = "<td width=\"25%\">"+ (fileInfoList[i].size)+"</td></tr>" ;
			body += td;

		}
		body +=  "</tbale></body></html>";
	}

	header = "HTTP/1.1 200 Ok\r\n";
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	header += "Server: petitwebserv\r\n";
	header += "Date: " + formatted_time() + "\r\n";
	header += "\r\n";
	header += body;
	
// _response_vec should be empty
	if (!_response_vec.empty())
		exit(42);
	_response_vec.insert(_response_vec.begin(), header.begin(), header.end());
	return ;
}


std::string  Response::create_header(void)
{
	std::string header;
	std::string resource =  _request->get_path();

	if (this->_status == 404)
	{
		header = "HTTP/1.1 404 Not found\r\n";
		header += "Content-Length: "+ std::to_string(this->_size) + "\r\n";
		header += "Server: petitwebserv\r\n";
		header += "Date: " + formatted_time() + "\r\n";
		header += "\r\n";
		return header;
	}
	else if (this->_status == 201)
	{
		header = "HTTP/1.1 201 Created\r\n";
		header += "Location: " + this->_request->get_path() + "\r\n";
		header += "Content-Length: "+ std::to_string(0) + "\r\n";
		header += "Server: petitwebserv\r\n";
		header += "Date: " + formatted_time() + "\r\n";
		header += "\r\n";
		return header;
	}
	else if (this->_status == 200)
	{
		header = "HTTP/1.1 200 OK\r\n";
		std::string extetion = getExtension(resource);

// header allow ressource to be downloaded to the client host machine.
		if (_index_file.size() == 0 && _mylocation->autoindex)
			header += "Content-Disposition: attachment\r\n";

		if (_request->get_method() != "DELETE")
			header += "Content-Type: " + std::string(MimeTypes::getType(extetion.c_str())) +"\r\n";
		header += "Content-Length: "+ std::to_string(this->_size) + "\r\n";
		header += "Server: petitwebserv\r\n";
		header += "Date: " + formatted_time() + "\r\n";
		header += "Set-Cookie: id="+_cookie+"; HttpOnly\r\n";
		header += "\r\n";
	}
	return header;
}

void	 Response::POST(void)
{
	std::cout << "Response::POST" << std::endl;
	std::string head_str;
	std::string upload_dir = _mylocation->upload;

	if (!upload_dir.size())
	{
		_response_vec = _405_error();
		return ;
	}

	// get path to uploading
	std::string body_path = _request->get_body_filename();
	std::string	upload_path = get_upload_path();
	std::string cmd = "mv " + body_path + " " + upload_path;

	// std::cerr << upload_path << std::endl;
	// std::cerr << body_path << std::endl;

	system(cmd.c_str());

	this->_status = 201;
	head_str = create_header();

	_response_vec.insert(_response_vec.begin(), head_str.begin(), head_str.end());
	return ;
}

void	 Response::GET(void)
{
	// std::cout << "\033[31;4mResponse::Get\033[0m" << std::endl;
	// if location has redirection

	//
	std::string resource = get_absolute_path();

	// std::cout << "resource = " << resource << std::endl;
	// 
	// std::cout << "Response::Get	resource = " << resource << "  |  " << _request->get_path() << std::endl;
	std::string extension = getExtension(resource);


	std::string		responce;
	std::string		str;
	std::string		body = "";
	std::streampos size;
	std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

	// std::cout << file.is_open() << std::endl;

	if (file.is_open())
	{

		this->_status = 200;
		file.close();
		std::vector<char> tow = getfileRaw(resource);
		this->_size = tow.size();
		responce = create_header();
		std::vector<char> first(responce.begin(), responce.end());
		first.insert(first.end(), tow.begin(), tow.end());
		this->_is_finshed = true;
		this->_response_vec = first;
		_response_vec = first;

	}
	else
	{
		_response_vec = _404_error();
	}

}

void	 Response::DELETE(void)
{
	std::string resource = get_absolute_path();
	std::string	cmd = "rm " + resource;


	int fd = open(resource.c_str(), O_RDONLY);
	if (fd > 0){
		close (fd);
	}
	else{
		_response_vec = _204_error();
		return ;
	}

	system(cmd.c_str());
	this->_status = 200;
	std::string body = "file deleted";
	this->_size = body.size();
	std::string header = create_header();

	std::vector<char> hamid(header.begin(), header.end());
	hamid.insert(hamid.end(), body.begin(), body.end());

	std::cerr << header << body << std::endl;
	_response_vec = hamid;
	return ;
}

void	 Response::CGI(void)
{
	return ;
}

bool				Response:: check_methods()
{
	return  std::count(_mylocation->methods.begin(), _mylocation->methods.end() , this->_request->get_method()) != 0;
}

// Method that's responsible for the all the Magic.
std::vector<char>	Response::serv()
{
	std::cout << "\033[32;1;4mResponse::serv\033[0m" << std::endl;
	std::cout <<  "srver name" << _request->_my_server->get_server_name()  << " " << _request->getHeader("Host") << std::endl;
	 find_location();

	_cookie = RandString(30);
	std::cerr << _request->get_method() << " " << _request->get_path() << " HTTP/" << _request->get_version() << std::endl;

	std::cerr << _request->get_bad_status() << "bad status"  << std::endl;
	if (_request->get_bad_status())
		return request_error();
	std::cerr << "loc "<< _mylocation << std::endl;
 	if (_mylocation == 0x0)
		return _404_error();

	std::string absolute_path = get_absolute_path();
	std::cerr << absolute_path << std::endl;
	if (isDirectory(absolute_path))
		this->find_index_file();
	std::cerr << (isDirectory(absolute_path) && !_mylocation->autoindex && !_mylocation->redirect.size() && !_index_file.size()) << "$$" << std::endl;
	if (isDirectory(absolute_path) && !_mylocation->autoindex && !_mylocation->redirect.size() && !_index_file.size())
		return _404_error();
	if (!check_methods())
		return _405_error();
	if (_mylocation->redirect.size() != 0)
		return create_303_header();

	// std::cout << "absolute path = " << absolute_path << std::endl;
	// std::cout << "location = " << _mylocation->url << std::endl;
	std::string extension = getExtension(this->_request->get_path());
	std::cerr << extension << "###" << std::endl;
	if (std::find(_mylocation->cgi.begin(), _mylocation->cgi.end(), extension) !=  _mylocation->cgi.end())
	{
		try
		{
			Cgi  cgi;
			cgi.set_cookie(_cookie);
			cgi.startCgi(this->_request, *_mylocation);
			if (cgi.StatusCode() == 504)
					return _504_error();
			else if (cgi.StatusCode() == 500)
				return _500_error();
			return cgi.readChunk();
		}
		catch(const std::exception& e)
		{
				return _500_error();
		}		
	}

	if (this->_request->get_method() ==  "POST")
	{
		POST();
	}
	else if (this->_request->get_method() == "GET")
	{
		if (isDirectory(absolute_path) && _index_file.size() != 0)
		{
			_request->set_path(_request->get_path() + _index_file);
			GET();
		}
		else if (isDirectory(absolute_path)  && this->_mylocation->autoindex)
			create_autoindex(this->_request->get_path());
		else if (isDirectory(absolute_path))
			return _403_error();
		else
			GET();
	}
	else if (this->_request->get_method() == "DELETE")
	{
		DELETE();
	}
	else{
		return _501_error();
	}
	return _response_vec;
}


void				Response::find_index_file(void)
{
	std::string root_file = _mylocation->root;

	for (size_t i = 0; i < _mylocation->index.size(); i++)
	{
		int fd;
		std::string absolute_path = root_file + "/" + _mylocation->index[i];
		fd = open(absolute_path.c_str(), O_RDONLY);
		if (fd > 0)
		{
			_index_file = _mylocation->index[i];
			close(fd);
			break ;
		}
	}
}


std::vector<char>	Response::create_302_header(void)
{
	this->close_connection = true;
	std::string s = "HTTP/1.1 303 See Other\r\nLocation: " + _mylocation->redirect + "\r\n";
	s += "Server: petitwebserv\r\n";
	s += "Date: " + formatted_time() + "\r\n";
	std::vector<char> ret(s.begin(), s.end());
	 return ret;
}


std::vector<char>	Response::create_303_header(void)
{
	this->close_connection = true;
	std::string s = "HTTP/1.1 303  Moved Permanently\r\nLocation: " + _mylocation->redirect + "\r\nConnection: close\nContent-Length: 0\r\n";
	s += "Server: petitwebserv\r\n";
	s += "Date: " + formatted_time() + "\r\n";
	std::vector<char> ret(s.begin(), s.end());
	 return ret;
}



std::string			Response::get_absolute_path(void)
{
	std::string uri = _request->get_path().erase(0, _mylocation->url.size()); 
	if (uri.size() && uri[0] != '/')
		uri.insert(uri.begin(), '/');

	return _mylocation->root + uri;
}

std::string			Response::get_upload_path(void)
{
	std::string uri = _request->get_path().erase(0, _mylocation->url.size());
	if (uri.size() && uri[0] != '/')
		uri.insert(uri.begin(), '/');
	
	return _mylocation->upload + uri;
}
std::vector<char>				Response::request_error(void)
{
	int status = _request->get_bad_status();

	if (status == 400)
		return _400_error();
	else if (status == 413)
		return _413_error();
	else if (status == 505)
		return _505_error();
	else if (status == 501)
		return _501_error();
	else
		exit(666);
}
/*	getter	*/

Request				*Response::get_request(void)
{
	return _request;
}

bool				Response::get_is_finished(void)
{
	return _is_finshed;
}
std::vector<char>	&Response::get_response_vec(void)
{
	return _response_vec;
}

size_t				Response::get_bytes_sent(void)
{
	return _bytes_sent;
}

void				Response::set_bytes_sent(size_t n)
{
	_bytes_sent = n;
}


void				Response::find_location(void)
{
	std::vector<struct location>  loc = _request->_my_server->get_locations();
	std::string req_path = this->_request->get_path();
	int location_index = -1;
	std::string matched_location = "/";

	for(size_t i = 0;  i < loc.size(); i++)
	{
		if (loc[i].url == "/")
		{
			location_index = i;
		}
		size_t ret = req_path.find(loc[i].url);
		std::cerr << loc[i].url << " " << req_path  << " | req_path[ret + loc[i].size() = " << (req_path[ret + loc[i].url.size()])  << std::endl;
		if (ret != std::string::npos && ret == 0 && (req_path[ret + loc[i].url.size()] == '/' || req_path == loc[i].url)){
			if (loc[i].url.length() >= matched_location.length()){
				matched_location = loc[i].url;
				location_index  = i;
			}
		}
	}
	if (location_index != -1){
		this->_mylocation = &_request->_my_server->get_locations()[location_index];
		std::cerr << "resolved location ==> " << _mylocation->url << std::endl;
	}
	else{
		_mylocation = 0x0;
	}
	std::cerr << "resolved location addr ==> " << _mylocation << std::endl;
}

//	/upload.html
//  /upload

