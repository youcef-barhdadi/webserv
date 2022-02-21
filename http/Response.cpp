#include <fstream>
#include <filesystem>
#include <vector>
#include "../Cgi/Cgi.hpp"
# include <unistd.h>
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
	this->bytes_sent = 0;
	this->is_finshed = false;
	this->_mylocation = NULL;
	this->_indexFile = "";
}
Response::Response(Request * req )
// 	// this->chanked_request = false;
{

// 	// this->is_finshed = true;

	this->request = req;
	this->bytes_sent = 0;
	this->is_finshed = false;
	this->_indexFile = "";
		// this->_mylocation = 
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
	this->response_vec.insert(this->response_vec.begin(), rhs.response_vec.begin(), rhs.response_vec.end());// =  rhs.response_vec;
	this->bytes_sent = rhs.bytes_sent;
	this->_size = rhs._size;
	this->is_finshed = rhs.is_finshed;
	this->_status = rhs._status;
	// this->request.set_path(rhs.request.get_path());
	this->request = rhs.request;
	this->_mylocation = rhs._mylocation;
	this->_indexFile = rhs._indexFile;
	return *this;
}


std::vector<char>  Response::AUTOINDEX(std::string path)
{
		std::string body = "<html lang='en'>\n<head>\n<title>Document</title>\n</head>\n<body>\n<h1>Index OF "+ path+ " </h1>\n<br>\n<table width=\"100%\">\n";
		std::string  header;
		std::string s = path[0] == '/' ? path.erase(0,1) : path; 
		if(isDirectory(path))
		{
				std::vector<FileInfo> fileInfoList = getListOfFiles(path);


				for(size_t i = 0; i < fileInfoList.size(); i++)
				{
					std::string td = "<tr><td width=\"50%\"> <a href=\"/"+ s + "/"+ fileInfoList[i].fileName +"\"> "+fileInfoList[i].fileName + "</a></td>"  ;
					body+= td;
						td  = "<td width=\"25%\">"+ fileInfoList[i].date + "</td>"  ;
						body += td;
						td = "<td width=\"25%\">"+ (fileInfoList[i].size)+"</td></tr>"  ;
						body += td;

				}
				body +=  "</tbale></body></html>";
		}

		header = "HTTP/1.1 200 Ok\n";
		header += "Content-Length: "+ std::to_string(body.size());
		header += "\n\n";
		header += body;
		// std::cout << header << std::endl;
		std::vector<char> resp(header.begin(), header.end());
		return resp;
}





std::string  Response::Create_Header()
{
	std::string header = "HTTP/1.1 200 OK\n";
	std::string resource =  request->get_path();

	if (this->_status == 404)
	{
		header = "HTTP/1.1  404 Not found \n";
		std::cout <<  "size " << this->_size << std::endl;
		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
		return header;
	}
	else if (this->_status == 201)	
	{
		header = "HTTP/1.1 201 Created\n";
		header += "Location: " + this->request->get_path();
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
	std::string upload_dir = "upload";	// config map["image"]
	// get path to uploading
	std::string body_path = request->get_body_filename();
	// copy 
	// std::cout << body_path << std::endl;
	 rename( body_path.c_str() , (upload_dir + request->get_path()).c_str());
	this->_status = 201;
	head_str = Create_Header();
	std::vector<char> resp_vec(head_str.begin(), head_str.end());
	return resp_vec;
}

/*
POST /upload/hamid.jpg 

location /upload root: /var/www


/var/www/hamod
*/

std::vector<char>	 Response::GET()
{
	if (this->is_finshed == true)
		return this->response_vec;
	std::string resource = request->get_path();
	std::string extension = getExtension(resource);
	if (extension == "pl")
	{
		Cgi cgi;
		std::string  strtest = cgi.startCgi(request);
		std::vector<char> test = handlCgiresponse(strtest);
		// exit(0);
		return  test;
	}	
	if (resource == "/")
	{
		resource = "index.html";
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
			this->is_finshed = true;
			this->response_vec = first;
			return first;
		}
		else 
		{
			// this means 404
			this->_status = 404;
  		  	std::vector<char> tow= getfileRaw("404.html");	
			this->_size = tow.size();
			responce = Create_Header();
  		  	std::vector<char> first(responce.begin(), responce.end());
			first.insert(first.end(), tow.begin(), tow.end());
			response_vec = first;
			this->is_finshed= true;
			return  first;
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
	std::vector<error_pages> err_pages = this->request->_server->get_error_pages();
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
	std::cout << get_errorpage(403) << std::endl;


	std::string l = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\nContent-Length: 106\n\r\n<html><head><title>403 Forbidden</title></head><body><center><h1>403 Forbidden</h1></center></body></html>";
	std::vector<char> res_vec(l.begin(), l.end());

	return res_vec;
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


bool				Response::check_methods()
{
	return  std::count(_mylocation->methods.begin(), _mylocation->methods.end() , this->request->get_method()) != 0;
}

std::vector<char>	Response::serv()
{
	this->find_location();
	this->find_index_file();
	

	if (!check_methods())
	{
		std::cout << "method" << std::endl;
		return _405_error();
	}

	std::cout << "index file: " << _indexFile << std::endl;
	if (this->request->get_method() ==  "POST")
	 	return 	POST();
	else if (this->request->get_method() == "GET")
	{
		if (isDirectory(this->request->get_path())   && _indexFile.size() != 0)
		{
			request->set_path(request->get_path() + _indexFile);
			return GET();
		}
		else if (isDirectory(this->request->get_path())  && this->_mylocation->autoindex)
			return AUTOINDEX(this->request->get_path());
		else if (isDirectory(this->request->get_path()))
			return _403_error();
		return GET();
	}
	std::cout << "error >> " << std::endl;
		// return GET();
	exit(1337);
}

void				Response::find_location(void)
{
	std::vector<struct location>  loc = this->request->_server->get_locations();
	std::string req_path = request->get_path();
	std::cout << "request path: " <<  request->get_path() << std::endl;
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
	this->_mylocation = &this->request->_server->get_locations()[location_index];

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


// std::vector<char>	Response::serv()
// {
// 	// first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 


	
// }


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */