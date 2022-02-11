#include "Response.hpp"
#include <fstream>
#include <filesystem>
#include <vector>
#include "../cgi/Cgi.hpp"
# include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>

#include "../utilities/utilities.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/



Response::Response( const Response & rhs)  : request(rhs.request)
{
	std::cout << "cpoy " << std::endl;
	*this= rhs;
}


Response::Response()
{
	this->bytes_sent = 0;
	this->is_finshed = false;


}
Response::Response(Request & req ) : request(req)
// 	// this->chanked_request = false;
{

// 	// this->is_finshed = true;


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
	this->response_vec = rhs.response_vec;
	this->bytes_sent = rhs.bytes_sent;
	this->_size = rhs._size;
	this->is_finshed = rhs.is_finshed;
	this->_status = rhs._status;
	// this->request = rhs.request;
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






std::string  Response::Creat_Header()
{
	std::string header = "HTTP/1.1 200 OK\n";
	std::string resource =  request.getPath();

	if (this->_status == 404)
	{
		header = "HTTP/1.1  404 Not found \n";
		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
		return header;
	}
	else if (this->_status == 200)
	{
		std::string extetion = getExtension(resource);
		std::cout  << extetion << std::endl;
	
		if (extetion == "css")
		{
			header += "Content-Type: text/css\n";
		}
		else if (extetion == "png")
		{
			header +=	"Content-Type: image/png\n";
		}
		else if (extetion == "jpg")
		{
			header +=  "Content-Type: image/jpg\n";
		}
		else if (extetion ==  "js")
		{
			header += "Content-Type: text/javascript\n";
		}
		else if (extetion  == "html")
		{
				header += "Content-Type: text/html\n";
		}
		else if (extetion == "mp4")
		{
			header += "Content-Type: video/mp4\n";
		}
		else {
			header += "Content-Type: text/html\n";
		}

		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
	}
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


std::vector<char> Response::Get(Request  &req, fd_set set)
{
	// size_t BUFFER_SIZE = 10000;
	// char BUFEFR[BUFFER_SIZE];
	// if (this->chanked_request == false)
	// {
	// 	this->requestedFileFD = open(this->req.getPath().c_str(), O_RDONLY);
	// 	this->sizeFile = FdGetFileSize(this->requestedFileFD);
	// 	// assert(this->requestedFileFD > 0);
	// }
	// // int ret = read(this->requestedFileFD, &BUFEFR, BUFFER_SIZE);
	// // BUFEFR[ret] = 0;
	// // std::vector<char> req(BUFEFR, BUFEFR + ret);

	// if (this->sended + ret > this->sizeFile   && this->req.getKeepALive() == false)
	// {
	// 	close(this->requestedFileFD);
	// 	FD_CLR(this->req.getConnectinFD(), &set);
	// }	

	throw std::logic_error("not implemented!");
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



std::vector<char>	Response::serv()
{
	// first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 


	if (this->is_finshed == true)
		return this->response_vec;


	std::string resource = request.getPath();
	std::string extension = getExtension(resource);
	// std::cout <<  "extension===>" << extension << std::endl;
	// this means is cgi
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
		exit(0);
		// throw std::logic_error("not implemented yet !");

}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */