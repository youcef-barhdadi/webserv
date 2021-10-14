#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <map>

class Server
{

	public:

		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );

		void create_server();


		void addPort(unsigned int port);
		std::vector<unsigned int> _listenfds;
		std::vector<unsigned int> socket_fd;
		std::vector<unsigned int> _ports;
		std::string _server_name;


		// clients ;
		std::map<unsigned int, unsigned int> clients;

		
	private:





};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */