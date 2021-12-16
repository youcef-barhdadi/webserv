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
		std::vector<char>	serv(Request & request, fd_set set);


		std::string buildResponse();
		std::string  Creat_Header(Request & request, std::string resource);

		void	handlPut(Request & request);
		std::vector<char> Get(Request  &req, fd_set set);

	private:

		size_t  _size;
		unsigned int _status;


		/**
		 * @brief 
		 * 	this refernce to the reqtuest object 
		 */

		Request &req;
		/**
		 * @brief 
		 * 	if the resposnse chnaked or not
		 */
		bool		chanked_request; 

		/**
		 * @brief 
		 * 		requested file  
		 */
		int requestedFileFD;


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */