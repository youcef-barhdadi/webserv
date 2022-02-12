#ifndef SPINNER_HPP
# define SPINNER_HPP

# include <iostream>
#include <fstream>
# include <string>
# include "../server/Server.hpp"
#include "../http/Request.hpp"
#include "../http/Response.hpp"
#include <unistd.h>

class Spinner 
{

	public:

		Spinner ();
		Spinner ( Spinner  const & src );
		~Spinner ();

		Spinner  &		operator=( Spinner  const & rhs );

		void	run();
		std::vector<Server *> _servers;

	private:

};

std::ostream &			operator<<( std::ostream & o, Spinner  const & i );

#endif /* ******************************************************** SPINNER _H */