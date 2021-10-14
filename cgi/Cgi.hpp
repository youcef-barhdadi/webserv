#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <string>
#include <sys/types.h>
#include <unistd.h>
#include "../http/Request.hpp"

class Cgi
{

	public:

		Cgi();
		Cgi( Cgi const & src );
		~Cgi();

		Cgi &		operator=( Cgi const & rhs );

		void startCgi(Request &request);


	private:

};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );

#endif /* ************************************************************* CGI_H */