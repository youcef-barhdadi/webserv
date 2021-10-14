#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include  "./Request.hpp"
#include <vector>

class Response
{

	public:

		Response();
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );



		// this method will do magic 
		std::vector<char>	serv(Request & request);


		std::string buildResponse();
		std::string  Creat_Header(Request & request, std::string resource);


	private:

		size_t  _size;
		unsigned int _status;


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */