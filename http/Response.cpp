#include <cassert>
#include "Response.hpp"
#include <fstream>
#include <filesystem>
#include <vector>
#include "../cgi/Cgi.hpp"
# include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/



Response::Response( const Response & src) : request(src.request)
{
}



Response::Response(Request * req ) : request(req)
{
	this->chanked_request = false;
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
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


int getSizeOfile(std::string file) {
   std::ifstream in_file(file, std::ios::binary);
   in_file.seekg(0, std::ios::end);
   int file_size = in_file.tellg();
	in_file.close();

	return file_size;
}



void	Response::handlPut(Request & request)
{



	
}




std::string	getExtension(std::string file)
{
	std::string str = file.substr(file.find('.')+1);

	return str;
} 


std::string  Response::Creat_Header()
{
	std::string header = "HTTP/1.1 200 OK\n";
	std::string resource =  request->getPath();

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
		else if (extetion ==  "js")
		{
			header += "Content-Type: text/javascript\n";
		}
		else if (extetion  == "html")
		{
				header += "Content-Type: text/html\n";
		}else {
			header += "Content-Type: text/html\n";
		}

		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
	}
	return header;
}



long FdGetFileSize(int fd)
{
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}



void		 Response::servGet(fd_set set)
{
	size_t BUFFER_SIZE = 10000;
	char BUFFER[BUFFER_SIZE];
	if (this->sended == 0)
	{
		std::string header = Creat_Header();

		// this fd is chekcked 
		write(this->request->getConnectinFD(), header.c_str(), header.size());
		this->sended = 1;
		return  ;
	}
	// i
	if (FD_ISSET(this->requestedFileFD, &set))
	{
		int ret = read(this->requestedFileFD, BUFFER, BUFFER_SIZE -1);
		BUFFER[ret] = '\0';
		write(this->request->getConnectinFD(), BUFFER, ret);
	}
}


void	Response::Get(fd_set set)
{
	size_t BUFFER_SIZE = 10000;
	char BUFEFR[BUFFER_SIZE];
	this->chanked_request = false;
	if (this->chanked_request == false)
	{
		this->requestedFileFD = open(this->request->getPath().c_str(), O_RDONLY);
		this->sizeFile = FdGetFileSize(this->requestedFileFD);
		// assert(this->requestedFileFD > 0);
		std::string header = Creat_Header();
		std::cout << header << std::endl;
		// this fd is chekcked 
		write(this->request->getConnectinFD(), header.c_str(), header.size());
		return ;
	}
	if (FD_ISSET(this->requestedFileFD, &set))
	{
		int ret = read(this->requestedFileFD, &BUFEFR, BUFFER_SIZE);
		assert(ret  > 0);
		write(this->request->getConnectinFD(), BUFEFR, ret);
	}




	// BUFEFR[ret] = 0;
	// std::vector<char> req(BUFEFR, BUFEFR + ret);

	// if (this->sended + ret > this->sizeFile   && this->request.getKeepALive() == false)
	// {
	// 	close(this->requestedFileFD);
	// 	FD_CLR(this->request.getConnectinFD(), &set);
	// }	
}















std::vector<char> getfileRaw(std::string file)
{
	std::ifstream iffile(file,  std::ios::in|std::ios::binary|std::ios::ate);
	std::streampos size;
	char* memblock;
	int sizee;
	// stup
	std::vector<char> oo;

	if (iffile.is_open())
	{
		size = iffile.tellg();
		sizee = size;
		memblock = new char [sizee];
		iffile.seekg (0, std::ios::beg);
		iffile.read (memblock, size);
		iffile.close();
		std::vector<char> ret(memblock, memblock+sizee);
		return ret;
	}
	return oo;

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





void		Response::serv(fd_set set)
{

	std::string method = this->request->getMethod();

	if (method == "GET")
	{
		this->Get(set);
		return ;
	}














	// // first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 
	// std::string resource = request.getPath();
	// std::string extension = getExtension(resource);
	// // std::cout <<  "extension===>" << extension << std::endl;
	// // this means is cgi
	// if (extension == "pl")
	// {
	// 	Cgi cgi;
	// 	std::string  strtest = cgi.startCgi(request);
	// 	std::vector<char> test = handlCgiresponse(strtest);
	// 	// exit(0);
	// 	return  test;
	// }	
	// if (resource == "/")
	// {
	// 	resource = "index.html";
	// }
	// else 
	// {
	// 	resource.erase(0, 1);
	// }
	// // std::cout << resource << std::endl;
	// 	std::string  responce;
	// 	// responce = Creat_Header(request, resource);
	// 	std::string  str;
	// 	std::string  body = "";
	// 	std::streampos size;
	// 	char* memblock;
	// 	std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

	// 	if (file.is_open())
	// 	{
	// 		this->_status = 200;
	// 		file.close();
  	// 	  	std::vector<char> tow= getfileRaw(resource);	
	// 		this->_size = tow.size();
	// 		responce = Creat_Header();
  	// 	  	std::vector<char> first(responce.begin(), responce.end());
	// 		first.insert(first.end(), tow.begin(), tow.end());
	// 		return first;

	// 	}
	// 	else {
	// 		// this means 404
	// 	this->_status = 404;
  	// 	  	std::vector<char> tow= getfileRaw("404.html");	
	// 		this->_size = tow.size();
	// 		responce = Creat_Header();
  	// 	  	std::vector<char> first(responce.begin(), responce.end());
	// 		first.insert(first.end(), tow.begin(), tow.end());

	// 		return  first;
	// 	}
	// 	// exit(0);


}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */