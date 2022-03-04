#include "Connection.hpp"


void  Connection::add_server(Server *server)
{
	this->_servers.push_back(server);
}



Connection::Connection(std::string host, int port)
{
	this->_host =host;
	this->_port = port;
}


void Connection::create_server()
{

	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	int enable = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		//std::cout << "BAD ADDRESS" << std::endl;
	}
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	if (server_fd < 0)
	{
		//std::cout << "BAD ADDRESS" << std::endl;
		exit(0);
	}
	this->_fd = server_fd;
	struct  sockaddr_in address;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, _host.c_str(), &(address.sin_addr));
	address.sin_port = htons(_port);
	int ret =  bind(server_fd, (struct sockaddr *)&address , sizeof(address));
	if (ret < 0)
	{
		perror("f");
		//std::cout << "Server  Error Check Port and Adress1" << std::endl;
		exit(0);
	}
	ret = listen(server_fd, 10000);
	if (ret < 0)
	{
		//std::cout << "Server  Error Check Port and Adress" << std::endl;
		exit(0);
	}
	
}