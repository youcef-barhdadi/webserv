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





std::vector<char>	Response::serv(Request & request)
{
	// first we need to check if this file exist and resolve the hole path if the path like this "/v1/prodcut/imags/dilodo1337.jpg" 
	std::string resource = request.getPath();

	// this code temporary 
	if (resource == "/")
	{
		resource = "index.html";
	}
	
	// std::cout << resource << std::endl;
	std::string  responce = "HTTP/1.1 200 OK\n";

	std::string  str;
	std::string  body = "";
	std::streampos size;
char* memblock;
	// if (!file.is_open()) 
	// {
	// 	// this means 404 response
	// 	std::cout << "file not find" << std::endl;
	// 	return "";
	// }
	if (resource == "/image.png")
	{	
		resource.erase(0, 1);
		std::ifstream file(resource,  std::ios::in|std::ios::binary|std::ios::ate);

		if (file.is_open())
		{

		responce += "Content-Type: image/png\n";


		    size = file.tellg();
			int sizee = size;
			memblock = new char [sizee];
			file.seekg (0, std::ios::beg);
			file.read (memblock, size);
			file.close();
			std::cout << "-----------------------" << std::endl; 		

			responce +=  "Content-Length: "+ std::to_string(size);
			responce += "\n\n";



			
  		  	std::vector<char> first(responce.begin(), responce.end());
  		  	std::vector<char> tow(memblock, memblock + size);
			first.insert(first.end(), tow.begin(), tow.end());
			std::cout << "-----------------------" << std::endl; 		
			std::cout << first.size() << "|||" << size << std::endl;
			return first;

		}
		std::cout<< "exiiist " << std::endl;
		exit(0);


	}
	else {
		responce += "Content-Type: text/html\n";
	}


		std::ifstream file(resource);


	   while (getline (file, str)) {
    // Output the text from the file
    		body +=  str;
    	}


		std::string number = std::to_string(body.length());
		responce +=   "Content-Length: " + number + "\n\n" +  body;


	file.close();


	// std::cout << responce << std::endl;


	std::vector<char> ret(responce.begin(), responce.end());
	return ret;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */