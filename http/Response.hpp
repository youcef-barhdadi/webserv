#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include  "./Request.hpp"
# include <vector>
# include "../MimeTypes/MimeTypes.h"
# include <algorithm>

class Response
{

	public:

		Response();
		Response(Request  *req );
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );



		// this method will do magic MAGIC
		std::vector<char>	serv();


		std::string buildResponse();
		std::string  Create_Header();

		std::vector<char> Get(Request  &req, fd_set set);



		std::vector<char>	 POST();
		std::vector<char>	 GET();
		std::vector<char>	 DELETE();
		std::vector<char>	 CGI();
		std::vector<char>	 AUTOINDEX(std::string str);


		void				find_location(void);
		void				find_index_file(void);
		

		bool				check_methods();
	
		std::string			get_errorpage(int status);
		std::vector<char> response_vec;

		size_t		bytes_sent;

		bool  is_finshed;
		Request	*request;
	private:
		std::vector<char> _403_error();
		std::vector<char> _405_error();

		size_t  _size;
		unsigned int _status;
		std::string _indexFile;
//	if the resposnse chanked or not
		bool		chanked_request; 
// 		requested file  
		int requestedFileFD;
		size_t sizeFile;
		size_t	sended;
		struct location *_mylocation;




};

#endif /* ******************************************************** RESPONSE_H */