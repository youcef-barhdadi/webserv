#include "Response.hpp"
#include <fstream>
#include <filesystem>
#include <vector>

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
	return header;
}








std::vector<char>	Response::serv(Request & request)
{
	// first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 
	std::string resource = request.getPath();

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
		responce = Creat_Header(request, resource);

		std::string  str;
		std::string  body = "";
		std::streampos size;
		char* memblock;

		std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

		if (file.is_open())
		{
			this->_status = 200;
		    size = file.tellg();
			int sizee = size;
			memblock = new char [sizee];
			file.seekg (0, std::ios::beg);
			file.read (memblock, size);
			file.close();
			std::cout << "-----------------------" << std::endl; 		
			this->_size = size;
			responce = Creat_Header(request, resource);
  		  	std::vector<char> first(responce.begin(), responce.end());
  		  	std::vector<char> tow(memblock, memblock + size);
			first.insert(first.end(), tow.begin(), tow.end());
			std::cout << "-----------------------" << std::endl; 		
			std::cout << first.size() << "|||" << size << std::endl;
			return first;

		}
		else {
			// this means 404
			this->_status = 404;
		}
		exit(0);

}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */