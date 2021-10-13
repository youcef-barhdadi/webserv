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
		std::vector<int> listenfd;
		std::string server_name;
		


};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */