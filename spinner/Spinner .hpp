#ifndef SPINNER _HPP
# define SPINNER _HPP

# include <iostream>
# include <string>
# include "../server/Server.hpp"

class Spinner 
{

	public:

		Spinner ();
		Spinner ( Spinner  const & src );
		~Spinner ();

		Spinner  &		operator=( Spinner  const & rhs );

		void	run();

	private:
		std::vector<Server> _servers;

};

std::ostream &			operator<<( std::ostream & o, Spinner  const & i );

#endif /* ******************************************************** SPINNER _H */