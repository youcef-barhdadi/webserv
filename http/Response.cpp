#include <fstream>
#include <filesystem>
#include <vector>
#include "../cgi/Cgi.hpp"
# include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include "Response.hpp"
#include "../utilities/utilities.hpp"
#include <cstring>
#include <stdio.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/



Response::Response( const Response & rhs)
{
	std::cout << "cpoy " << std::endl;
	*this= rhs;
}


Response::Response()
{
	this->bytes_sent = 0;
	this->is_finshed = false;


}
Response::Response(Request * req )
// 	// this->chanked_request = false;
{

// 	// this->is_finshed = true;

	this->request = req;
	this->bytes_sent = 0;
	this->is_finshed = false;
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
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Response const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/




void	Response::handlPut(Request & request)
{



	
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
		std::cout << header << std::endl;
		std::vector<char> resp(header.begin(), header.end());
		return resp;
}





std::string  Response::Creat_Header()
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
		std::cout  << extetion << std::endl;
	
		std::cout << "extetion.c_str() . "  << extetion.c_str() << std::endl;
		header += "Content-Type: " + std::string(MimeTypes::getType(extetion.c_str())) +"\n";

		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
	}
	std::cout << header << std::endl;
	return header;
}


// std::vector<char> Response::servGet(fd_set set)
// {
// 	// size_t BUFFER_SIZE = 10000;
// 	// char BUFFER[BUFFER_SIZE];
// 	// if (this->sended == 0)
// 	// {
// 	// 	std::string header = Creat_Header();

// 	// 	// this fd is chekcked 
// 	// 	write(this->request.getConnectinFD(), header.c_str(), header.size());
// 	// 	this->sended = 1;
// 	// 	return ;
// 	// }
// 	// // i
// 	// if (FD_ISSET(this-requestedFileFD, &set))
// 	// {
// 	// 	int ret = read(this->requestedFileFD, BUFFER, BUFFER_SIZE -1);
// 	// 	BUFFER[ret] = '\0';
// 	// 	write(this->request.getConnectinFD(), BUFFER, ret);
// 	// }
// 	throw std::logic_error("not implmented!");
// }


// std::vector<char> Response::Get(Request  &req, fd_set set)
// {
// 	// size_t BUFFER_SIZE = 10000;
// 	// char BUFEFR[BUFFER_SIZE];
// 	// if (this->chanked_request == false)
// 	// {
// 	// 	this->requestedFileFD = open(this->req.getPath().c_str(), O_RDONLY);
// 	// 	this->sizeFile = FdGetFileSize(this->requestedFileFD);
// 	// 	// assert(this->requestedFileFD > 0);
// 	// }
// 	// // int ret = read(this->requestedFileFD, &BUFEFR, BUFFER_SIZE);
// 	// // BUFEFR[ret] = 0;
// 	// // std::vector<char> req(BUFEFR, BUFEFR + ret);

// 	// if (this->sended + ret > this->sizeFile   && this->req.getKeepALive() == false)
// 	// {
// 	// 	close(this->requestedFileFD);
// 	// 	FD_CLR(this->req.getConnectinFD(), &set);
// 	// }	

// 	throw std::logic_error("not implemented!");
// }






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
	std::cout << body_path << std::endl;
	 rename( body_path.c_str() , (upload_dir + request->get_path()).c_str());
	this->_status = 201;
	head_str = Creat_Header();
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
		// responce = Creat_Header(request, resource);
		std::string  str;
		std::string  body = "";
		std::streampos size;
		char* memblock;
		std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

		if (file.is_open())
		{

			this->_status = 200;
			file.close();
  		  	std::vector<char> tow= getfileRaw(resource);
			this->_size = tow.size();
			std::cout << "size is >> " << this->_size << std::endl;
			responce = Creat_Header();
  		  	std::vector<char> first(responce.begin(), responce.end());
			first.insert(first.end(), tow.begin(), tow.end());
			this->is_finshed = true;
			this->response_vec = first;
			return first;
		}
		else {
			// this means 404
			this->_status = 404;
  		  	std::vector<char> tow= getfileRaw("404.html");	
			this->_size = tow.size();
			responce = Creat_Header();
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

std::vector<char>	Response::serv()
{
	if (this->request->get_method() ==  "POST")
	{
	 	return 	POST();
	}
	else if (this->request->get_method() == "GET")
	{
		if (isDirectory(this->request->get_path()))
			return  AUTOINDEX(this->request->get_path()); 
		return GET();
	}
	std::cout << "error >> " << std::endl;
		// return GET();
	exit(1337);
}






// std::vector<char>	Response::serv()
// {
// 	// first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 


	
// }


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */