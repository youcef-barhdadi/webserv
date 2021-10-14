#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
}

Server::Server( const Server & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


void Server::create_server()
{

			  std::cout << "eroror  ===> " <<  this->_ports.size()<< std::endl;

	for (int i = 0; i <  this->_ports.size(); i++)
	{

		int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0)
		{
			perror("error");
			exit(0);
		}
		this->socket_fd.push_back(server_fd);
		std::cout << "socket fd ==> ||"<< this->socket_fd.size() << std::endl;
		struct  sockaddr_in address;
		memset((char *)&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr= INADDR_ANY;
		address.sin_port =htons(this->_ports[i]);
		int ret =  bind(server_fd, (struct sockaddr *)&address , sizeof(address));
		if (ret < 0)
		{
			perror("error ");
			return ;
		}
		 ret = listen(server_fd, 1000);
		if (ret < 0)
		{
			perror("error ");
			return ;
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void Server::addPort(unsigned int port)
{
	this->_ports.push_back(port);
	std::cout<< this->_ports.size() << std::endl;
}



/* ************************************************************************** */