#ifndef SPINNER_HPP
# define SPINNER_HPP

# include <iostream>
#include <fstream>
# include <string>
# include "../Server/Server.hpp"
#include "../Http/Request.hpp"
#include "../Http/Response.hpp"
#include <unistd.h>

class Spinner 
{

	public:

		Spinner ();
		~Spinner ();

		Spinner  &		operator=( Spinner  const & rhs );

		void	run();
		std::vector<Server *> _servers;

	private:

};


#endif /* ******************************************************** SPINNER _H */