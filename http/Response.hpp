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
		std::vector<char>	serv();


		std::string buildResponse();
		std::string  Creat_Header();

		void	handlPut(Request & request);
		std::vector<char> Get(Request  &req, fd_set set);


		
		bool  is_finshed;

		std::vector<char> response_vec;



		size_t		bytes_sent;

	private:



		std::vector<char> servGet();

		size_t  _size;
		unsigned int _status;


		/**
		 * @brief 
		 * 	this refernce to the reqtuest object 
		 */

		Request request;
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



		size_t sizeFile;

		size_t	sended;


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */