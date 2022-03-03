#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <string>
#include <sys/types.h>
#include <unistd.h>
#include "../Http/Request.hpp"

class Cgi
{

	public:

		Cgi();
		~Cgi();

		Cgi &		operator=( Cgi const & rhs );
		std::string		startCgi(Request *request,  location location);
		std::vector<char>	readChunk();
		int pip[2];

	private:
		bool IsTimeOut;
		bool	NotExist;
		int		StatusCode();

};

#endif /* ************************************************************* CGI_H */