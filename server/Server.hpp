#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
#include <vector>

class Server
{

	public:

		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );

	private:
		std::vector<unsigned int> _listenfds;
		std::vector<unsigned int> _ports;
		std::string _server_name;
		




};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */