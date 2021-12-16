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
		Response(Request & req );
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );



		// this method will do magic 
		std::vector<char>	serv(Request & request);


		std::string buildResponse();
		std::string  Creat_Header(Request & request, std::string resource);

		void	handlPut(Request & request);


	private:

		size_t  _size;
		unsigned int _status;

		Request &req;


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */