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


/*
 ** ------------------------------- CONSTRUCTOR --------------------------------
 */



Response::Response( const Response & rhs)
{
	*this= rhs;
}


Response::Response()
{
	this->_bytes_sent = 0;
	this->_is_finshed = false;
	this->_mylocation = NULL;
	this->_indexFile = "";
}
Response::Response(Request * req )
{
	this->_request = req;
	this->_bytes_sent = 0;
	this->_is_finshed = false;
	this->_indexFile = "";
}
/*
 ** -------------------------------- DESTRUCTOR --------------------------------
 */

Response::~Response()
{
}


/*
 ** --------------------------------- OVERLOAD ---------------------------------
 */

Response &				Response::operator=( Response const & rhs )
{
	this->_response_vec.insert(this->_response_vec.begin(), rhs._response_vec.begin(), rhs._response_vec.end());// =  rhs.response_vec;
	this->_bytes_sent = rhs._bytes_sent;
	this->_size = rhs._size;
	this->_is_finshed = rhs._is_finshed;
	this->_status = rhs._status;
	this->_request = rhs._request;
	this->_mylocation = rhs._mylocation;
	this->_indexFile = rhs._indexFile;
	return *this;
}


std::vector<char>  Response::AUTOINDEX(std::string path)
{
	if ( path[ path.size() - 1] != '/')
		path.insert( path.end(), '/');
	std::string	body = "<html lang='en'>\n<head>\n<title>Document</title>\n</head>\n<body>\n<h1>Index OF "+ path + " </h1>\n<br>\n<table width=\"100%\">\n<hr>";
	std::string  header;
	std::string s = path[0] == '/' ? path.erase(0,1) : path;
	if (s[s.size()-1] == '/')
		s.erase(s.end() - 1);
	if(isDirectory(path))
	{
		std::vector<FileInfo> fileInfoList = getListOfFiles(path);


		for(size_t i = 0; i < fileInfoList.size(); i++)
		{
			std::string td = "<tr><td width=\"50%\"> <a href=\"/"+ s + "/"+ fileInfoList[i].fileName +"\"> "+fileInfoList[i].fileName + "</a></td>"  ;
			body += td;
			if (fileInfoList[i].fileName == ".." || fileInfoList[i].fileName == ".")
				continue;
			td  = "<td width=\"25%\">"+ fileInfoList[i].date + "</td>"  ;
			body += td;
			if (isDirectory( s + "/"+ fileInfoList[i].fileName))
				td = "<td width=\"25%\">"+ std::string("-") +"</td></tr>"  ;
			else
				td = "<td width=\"25%\">"+ (fileInfoList[i].size)+"</td></tr>" ;
			body += td;

		}
		body +=  "</tbale></body></html>";
	}

	header = "HTTP/1.1 200 Ok\n";
	header += "Content-Type: text/html\n";
	// header += "Content-Disposition: attachment\n";
	header += "Content-Length: "+ std::to_string(body.size());
	header += "\n\r\n";
	header += body;
	std::vector<char> resp(header.begin(), header.end());
	return resp;
}





std::string  Response::Create_Header()
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
		// std::cout  << extetion << std::endl;

		// std::cout << "extetion.c_str() . "  << extetion.c_str() << std::endl;
		header += "Content-Type: " + std::string(MimeTypes::getType(extetion.c_str())) +"\n";

		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
	}
	// else if (this->_status == 403)

	// std::cout << header << std::endl;
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


void		serve_chanked()
{
	// if the size is bigger the Buffer_size
}



std::vector<char>	 Response::POST()
{
	std::string head_str;
	std::string upload_dir = _mylocation->upload.second;

	// get path to uploading
	std::string body_path = _request->get_body_filename();

	rename(body_path.c_str() , (upload_dir + _request->get_path()).c_str());
	this->_status = 201;
	head_str = Create_Header();
	std::vector<char> resp_vec(head_str.begin(), head_str.end());
	return resp_vec;
}

std::vector<char>	 Response::GET()
{
	if (this->_is_finshed == true)
		return this->_response_vec;
	std::string resource = _request->get_path();
	std::string extension = getExtension(resource);
	if (extension == "pl")
	{
		Cgi cgi;
		std::string  strtest = cgi.startCgi(_request);
		std::vector<char> test = handlCgiresponse(strtest);
		// exit(0);
		return  test;
	}
	else
	{
		resource.erase(0, 1);
	}
	// std::cout << resource << std::endl;
	std::string  responce;
	// responce = Createe_Header(request, resource);
	std::string  str;
	std::string  body = "";
	std::streampos size;
	std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

	if (file.is_open())
	{

		this->_status = 200;
		file.close();
		std::vector<char> tow= getfileRaw(resource);
		this->_size = tow.size();
		responce = Create_Header();
		std::vector<char> first(responce.begin(), responce.end());
		first.insert(first.end(), tow.begin(), tow.end());
		this->_is_finshed = true;
		this->_response_vec = first;
		return first;
	}
	else
	{
		return _404_error();
	}
	// throw std::logic_error("not implemented yet !");

}

std::vector<char>	 Response::DELETE()
{
	std::vector<char> t;

	return t;
}

std::vector<char>	 Response::CGI()
{
	std::vector<char> t;


	return t;
}


std::string		Response::get_errorpage(int status)
{
	std::vector<error_pages> err_pages = this->_request->_server->get_error_pages();
	std::string			path = "";

	for (size_t i = 0; i < err_pages.size(); i++)
	{
		if (std::count(err_pages[i].status_codes.begin(), err_pages[i].status_codes.end(), status) != 0)
		{
			path = err_pages[i].root;
			break;
		}
	}
	return path;

}


std::vector<char> Response::_403_error()
{
	std::string path =  get_errorpage(403);
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
	std::string path =  get_errorpage(405);
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
	std::string path =  get_errorpage(404);
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

bool				Response::check_methods()
{
	return  std::count(_mylocation->methods.begin(), _mylocation->methods.end() , this->_request->get_method()) != 0;
}


// Method that's responsible for the all the Magic.
std::vector<char>	Response::serv()
{
	std::cout << "request path: " <<  _request->get_path() << std::endl;
	this->find_location();
	if (isDirectory(this->_request->get_path()))
		this->find_index_file();
	if (!check_methods())
		return _405_error();
	// no location was found
	if (_mylocation == 0x0)
		return _404_error();

	if (this->_request->get_method() ==  "POST")
	{
		// if the location does not support upload
		if (!_mylocation->upload.first)
			return _404_error();
		return 	POST();
	}
	else if (this->_request->get_method() == "GET")
	{
		if (isDirectory(this->_request->get_path())   && _indexFile.size() != 0)
		{
			_request->set_path(_request->get_path() + _indexFile);
			return GET();
		}
		else if (isDirectory(this->_request->get_path())  && this->_mylocation->autoindex)
			return AUTOINDEX(this->_request->get_path());
		else if (isDirectory(this->_request->get_path()))
			return _403_error();
		return GET();
	}
	exit(1337);
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
			_indexFile = _mylocation->index[i];
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
