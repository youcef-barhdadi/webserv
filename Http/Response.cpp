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

	std::string t_path = path;
	t_path.erase(t_path.begin());
	if (t_path.find('/') != std::string::npos)
		t_path = t_path.substr(t_path.find('/'), t_path.size());
	t_path.erase(t_path.end() - 1);
	std::cout << "t_path = " << t_path << std::endl;
// static/www		_locations->root + /www  ..

	std::string	body = "<html lang='en'>\n<head>\n<title>Document</title>\n</head>\n<body>\n<h1>Index OF "+ path + " </h1>\n<br>\n<table width=\"100%\">\n<hr>";
	std::string  header;
	std::string s = path[0] == '/' ? path.erase(0,1) : path;
	// std::string s = path;
	if (s[s.size()-1] == '/')
		s.erase(s.end() - 1);

	if(isDirectory(_mylocation->root + t_path))
	{
// here	
		std::vector<FileInfo> fileInfoList = getListOfFiles(_mylocation->root + t_path);


		for(size_t i = 0; i < fileInfoList.size(); i++)
		{
			std::string td = "<tr><td width=\"50%\"> <a href=\"/"+ s + "/"+ fileInfoList[i].fileName +"\"> "+fileInfoList[i].fileName + "</a></td>"  ;
			body += td;
			if (fileInfoList[i].fileName == ".." || fileInfoList[i].fileName == ".")
				continue;
			td  = "<td width=\"25%\">"+ fileInfoList[i].date + "</td>"  ;
			body += td;
			if (isDirectory( _mylocation->root + t_path + "/"+ fileInfoList[i].fileName))
				td = "<td width=\"25%\">"+ std::string("-") +"</td></tr>"  ;
			else
				td = "<td width=\"25%\">"+ (fileInfoList[i].size)+"</td></tr>" ;
			body += td;

		}
		body +=  "</tbale></body></html>";
	}

	header = "HTTP/1.1 200 Ok\n";
	header += "Content-Type: text/html\n";
	header += "Content-Length: "+ std::to_string(body.size());
	header += "\n\r\n";
	header += body;
	
// _response_vec should be empty
	if (!_response_vec.empty())
		exit(42);
	_response_vec.insert(_response_vec.begin(), header.begin(), header.end());
	return ;
}





std::string  Response::create_header(void)
{
	std::string header = "HTTP/1.1 200 OK\n";
	std::string resource =  _request->get_path();

	if (this->_status == 404)
	{
		header = "HTTP/1.1 404 Not found \n";
		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
		return header;
	}
	else if (this->_status == 201)
	{
		header = "HTTP/1.1 201 Created\n";
		header += "Location: " + this->_request->get_path();
		header += "\nContent-Length: "+ std::to_string(0);

		header += "\n\r\n";
		return header;
	}
	else if (this->_status == 200)
	{
		std::string extetion = getExtension(resource);

// header allow ressource to be downloaded to the client host machine.
		if (_mylocation->autoindex)
			header += "Content-Disposition: attachment\n";

		header += "Content-Type: " + std::string(MimeTypes::getType(extetion.c_str())) +"\n";
		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
	}
	return header;
}


std::vector<char> handlCgiresponse(std::string & str)
{

	std::vector<std::string> strs = split(str, '\n');;

	int size = strs[2].length();


	std::string content = "HTTP/1.1 200 OK\nContent-Length: "+ std::to_string(size);
	content += "\n";
	content.append(str);

	std::vector <char> vec(content.begin(), content.end());


	return vec;
}

void	 Response::POST(void)
{
	std::string head_str;
	std::string upload_dir = _mylocation->upload;

	// get path to uploading
	std::string body_path = _request->get_body_filename();

	rename(body_path.c_str() , (upload_dir + _request->get_path()).c_str());
	this->_status = 201;
	head_str = create_header();

	_response_vec.insert(_response_vec.begin(), head_str.begin(), head_str.end());
	return ;
}

void	 Response::GET(void)
{
	std::cout << "Response::Get" << std::endl;
	//
	std::string resource;
	if (!_mylocation->autoindex){
		resource = _mylocation->root + _request->get_path();
	}
	else{
		std::string t_path = _request->get_path();
		t_path.erase(t_path.begin());
		if (t_path.find('/') != std::string::npos)
			t_path = t_path.substr(t_path.find('/'), t_path.size());
		resource = _mylocation->root + t_path;
	}
	
	// std::cout << "Response::Get	resource = " << resource << "  |  " << _request->get_path() << std::endl;
	std::string extension = getExtension(resource);
	if (extension == "pl")
	{
		Cgi cgi;
		std::string  strtest = cgi.startCgi(_request);
		std::vector<char> test = handlCgiresponse(strtest);
		return  ;
	}

	std::string  responce;
	std::string  str;
	std::string  body = "";
	std::streampos size;
	std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

	std::cout << file.is_open() << std::endl;

	if (file.is_open())
	{

		this->_status = 200;
		file.close();
		std::vector<char> tow= getfileRaw(resource);
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
	return ;
}

void	 Response::CGI(void)
{
	return ;
}


std::vector<char> Response::_403_error()
{
	std::string path =  _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\n";
	if (path.size() == 0)
	{
		std::string ll = "Content-Length: 106\n\r\n<html><head><title>403 Forbidden</title></head><body><center><h1>403 Forbidden</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}
	std::vector<char> file_vec = getfileRaw(path + "/403.html");

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char> Response::_405_error()
{
	std::string path =  _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\n";
	if (path.size() == 0)
	{
		std::string ll = "Content-Length: 125\n\r\n<html><head><title>405 Method Not Allowed</title></head><body><center><h1>405  Method Not Allowed</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}
	std::vector<char> file_vec = getfileRaw(path + "/405.html");

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;

}

std::vector<char>	Response::_404_error()
{
	std::string path =  _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\n";
	if (path.size() == 0)
	{
		std::string ll = "Content-Length: 106\n\r\n<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}
	std::vector<char> file_vec = getfileRaw(path + "/404.html");

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char>	Response::_501_error()
{
	std::string path = _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\n";
	if (path.size() == 0)
	{
		std::string ll = "Content-Length: 118\n\r\n<html><head><title>501 Not Implemented</title></head><body><center><h1>501 Not Implemented</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}
	std::vector<char> file_vec = getfileRaw(path + "/501.html");

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}


bool				Response:: check_methods()
{
	return  std::count(_mylocation->methods.begin(), _mylocation->methods.end() , this->_request->get_method()) != 0;
}


// Method that's responsible for the all the Magic.
std::vector<char>	Response::serv()
{
	std::cout << "request path: " <<  _request->get_path() << std::endl;
	this->find_location();
	std::cout << "location url: " << _mylocation->url << std::endl;
	if (_mylocation == 0x0)
		return _404_error();
	if (isDirectory(this->_request->get_path()))
		this->find_index_file();
	std::cout << "index file: " << _index_file << std::endl;
	if (!check_methods())
		return _405_error();
	// no location was found

//
	std::string t_path = _request->get_path();


	if (_mylocation->autoindex){
		t_path += "/";
		t_path.erase(t_path.begin());
		if (t_path.find('/') != std::string::npos)
			t_path = t_path.substr(t_path.find('/'), t_path.size());
		t_path.erase(t_path.end()-1);
	}

//
	if (this->_request->get_method() ==  "POST")
	{
		// if the location does not support upload
		if (!_mylocation->upload.size())
			return _404_error();
		POST();
	}
	else if (this->_request->get_method() == "GET")
	{
//	this->_request->get_path()	_mylocation->root
		std::cout << "host path: " << _mylocation->root + t_path << std::endl;
		if (isDirectory(_mylocation->root + t_path)   && _index_file.size() != 0)
		{
			_request->set_path(_request->get_path() + _index_file);
			GET();
		}
		else if (isDirectory(_mylocation->root + t_path)  && this->_mylocation->autoindex)
			create_autoindex(this->_request->get_path());
		else if (isDirectory(_mylocation->root + t_path))
			return _403_error();
		else
			GET();
	}
	else{
		return _501_error();
	}
	return _response_vec;
}

void				Response::find_location(void)
{
	std::vector<struct location>  loc = this->_request->_server->get_locations();
	std::string req_path = _request->get_path();
	int location_index = 0;
	std::string matched_location = "";

	for(size_t i = 0;  i < loc.size(); i++)
	{
		size_t ret = req_path.find(loc[i].url);
		if (ret != std::string::npos && ret == 0){
			if (loc[i].url.length() > matched_location.length()){
				matched_location = loc[i].url;
				location_index  = i;
			}
		}
	}
	this->_mylocation = &this->_request->_server->get_locations()[location_index];

	// std::cout << "matched_location = " << matched_location << " | with index = " << location_index << std::endl;
}

void				Response::find_index_file(void)
{
	std::string root_file = _mylocation->root;

	for (size_t i = 0; i < _mylocation->index.size(); i++)
	{
		int fd = 0;
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
