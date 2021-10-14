#include "Response.hpp"
#include <fstream>
#include <filesystem>
#include <vector>
#include "../cgi/Cgi.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
{
}

Response::Response( const Response & src )
{
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



std::string getExtension(std::string file)
{
	std::string str = file.substr(file.find('.')+1);

	return str;
} 


std::string  Response::Creat_Header(Request & request, std::string resource)
{
	std::string header = "HTTP/1.1 200 OK\n";


	if (this->_status == 404)
	{
		header = "HTTP/1.1  404 Not found \n";
		header += "Content-Length: "+ std::to_string(this->_size);
		header += "\n\n";
		return header;
	}
	else if (this->_status = 200)
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




std::vector<char>	Response::serv(Request & request)
{
	// first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 
	std::string resource = request.getPath();

	std::string extension = getExtension(resource);
	std::cout <<  "extension===>" << extension << std::endl;
	// this means is cgi
	if (extension == "pl")
	{
	std::cout <<  "enter" << std::endl;


			Cgi cgi;
			cgi.startCgi(request);
			std::vector<char> test ;
			exit(0);
		return  test;
	}




	// this code temporary 
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
			responce = Creat_Header(request, resource);
  		  	std::vector<char> first(responce.begin(), responce.end());
			first.insert(first.end(), tow.begin(), tow.end());
			return first;

		}
		else {
			// this means 404
		this->_status = 404;
  		  	std::vector<char> tow= getfileRaw("404.html");	
			this->_size = tow.size();
			responce = Creat_Header(request, "404.html");
  		  	std::vector<char> first(responce.begin(), responce.end());
			first.insert(first.end(), tow.begin(), tow.end());

			return  first;
		}
		// exit(0);


}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */