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
		Response(Request  *req );
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );



		// this method will do magic 
<<<<<<< HEAD
		void		serv(fd_set set);
=======
		std::vector<char>	serv();
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343


		std::string buildResponse();
		std::string Creat_Header();
		void		handlPut(Request & request);
		void		Get(fd_set set);
		void 		servGet(fd_set set);


		
		bool  is_finshed;

		std::vector<char> response_vec;



		size_t		bytes_sent;

		Request request;
	private:



		std::vector<char> servGet();

		size_t  _size;
		unsigned int _status;


		/**
		 * @brief 
		 * 	this refernce to the reqtuest object 
		 */

<<<<<<< HEAD
		Request *request;
=======
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
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