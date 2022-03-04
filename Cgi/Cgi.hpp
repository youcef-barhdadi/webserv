#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <string>
#include <sys/types.h>
#include <unistd.h>
#include "../Http/Request.hpp"
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

class Cgi
{

	public:

		Cgi();
		~Cgi();

		Cgi &		operator=( Cgi const & rhs );
		std::string		startCgi(Request *request,  location location);
		std::vector<char>	readChunk();
		int pip[2];
		int		StatusCode();

		void	set_cookie(std::string const &Cookie);

	private:
		bool IsTimeOut;
		bool	NotExist;
		bool IServerError;
		std::string cookie;

};

#endif /* ************************************************************* CGI_H */