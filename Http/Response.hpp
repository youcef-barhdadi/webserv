#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include  "./Request.hpp"
# include <vector>
# include "../MimeTypes/MimeTypes.h"
# include <algorithm>
# include "../Utilities/Utilities.hpp"

class Response
{

	public:

		Response();
		Response(Request  *req );
		Response( Response const & src );
		~Response();

		Response &			operator=( Response const & rhs );



		// this method will do magic MAGIC
		std::vector<char>	serv(void);

		std::string 		create_header(void);
		std::string			create_status_line(std::string status_code);

		std::vector<char>	create_302_header(void);
		std::vector<char>	create_303_header(void);
		void				 POST(void);
		void				 GET(void);
		void				 DELETE(void);
		void				 CGI(void);


		std::vector<char>	request_error(void);

		void				create_autoindex(std::string str);
		void				find_index_file(void);


		bool				check_methods();

		std::string			get_absolute_path(void);
		std::string			get_upload_path(void);

		Request				*get_request(void);
		bool				get_is_finished(void);
		std::vector<char>	&get_response_vec(void);
		size_t				get_bytes_sent(void);

		void				set_bytes_sent(size_t n);



		bool		close_connection;

	private:
// error pages
		std::vector<char> _505_error();
		std::vector<char> _400_error();
		std::vector<char> _403_error();
		std::vector<char> _404_error();
		std::vector<char> _405_error();
		std::vector<char> _413_error();
		std::vector<char> _501_error();
		std::vector<char> _204_error();

		std::vector<char> _504_error();
		std::vector<char> _500_error();
void				find_location(void);
//
// 204 (No Content)

		Request				*_request;
		bool				_is_finshed;
		std::vector<char>	_response_vec;
		std::string			_index_file;
		location			*_mylocation;
		size_t				_bytes_sent;

		std::string			_cookie;

//	not used yet
		size_t  _size;
		unsigned int _status;
//	if the resposnse chunked or not
		// bool		chunked_request;
// 		requested file
		// int requestedFileFD;
		// size_t sizeFile;
		// size_t	sended;
//		location
};

#endif /* ******************************************************** RESPONSE_H */
