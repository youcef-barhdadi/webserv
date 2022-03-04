//
//  Server.cpp  7 Feb 2020
//

#include "Server.hpp"

Server::Server(void)
: _host(""), _server_name(""), _error_pages(" "), _client_body_size(0)
{

}

Server::~Server(void)
{

}

void	Server::set_server_name(std::string &server_name){
	this->_server_name = server_name;
}
void	Server::set_port(std::vector<unsigned int> &port){
	this->_ports = port;
}
void	Server::set_host(std::string &host){
	this->_host = host;
}
void	Server::set_error_pages(std::string &error_pages){
	_error_pages = error_pages;
}
void	Server::set_client_body_size(size_t &client_body_size){
	this->_client_body_size = client_body_size;
}
void	Server::set_locations(struct location &location){
	this->_locations.push_back(location);
}


void	Server::debug(void)
{
	std::cout << std::string(50, '-') << std::endl;
	std::cout << "server_name: " << _server_name << std::endl;
	std::cout << "host: " << _host << std::endl;
	std::cout << "port: "; for(size_t i=0; i<_ports.size(); i++){std::cout << _ports[i] << " ";}; std::cout << '\n';
	std::cout << "error_pages: " << _error_pages << std::endl;
	std::cout << "client_body_size: " << _client_body_size << std::endl;
	for (size_t i=0; i<_locations.size(); i++)
	{
		std::cout << std::endl << "location nb " << i + 1 << std::endl;
		std::cout << "\t" << "url: " << _locations[i].url << std::endl;
		std::cout << "\t" << "methods: ";for(size_t j=0;j<_locations[i].methods.size(); j++){std::cout <<_locations[i].methods[j] <<" "; };std::cout << "\n";
		std::cout << "\t" << "root: " << _locations[i].root << std::endl;
		std::cout << "\t" << "index: ";for(size_t j=0; j<_locations[i].index.size(); j++){std::cout << _locations[i].index[j] << " ";}; std::cout << "\n";
		std::cout << "\t" << "autoindex: " << _locations[i].autoindex << std::endl;
		std::cout << "\t" << "upload: " << _locations[i].upload << std::endl;
		std::cout << "\t" << "cgi: " << _locations[i].cgi.size() << std::endl;
		std::cout << "\t" << "redirect: " << _locations[i].redirect << std::endl;
	}
	std::cout << std::string(50, '&') << std::endl;
}


void Server::create_server()
{
	for (size_t i = 0; i <  this->_ports.size(); i++)
	{
		int	server_fd = socket(AF_INET, SOCK_STREAM, 0);

		int enable = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		{
			std::cout << "BAD ADDRESS" << std::endl;
		}
		fcntl(server_fd, F_SETFL, O_NONBLOCK);
		if (server_fd < 0)
		{
			std::cout << "BAD ADDRESS" << std::endl;
			exit(0);
		}
		this->_socket_fd.push_back(server_fd);
		struct  sockaddr_in address;
		memset((char *)&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		inet_pton(AF_INET, _host.c_str(), &(address.sin_addr));
		address.sin_port = htons(this->_ports[i]);
		int ret =  bind(server_fd, (struct sockaddr *)&address , sizeof(address));
		if (ret < 0)
		{
			std::cout << "Server  Error Check Port and Adress" << std::endl;
			exit(0);
		}
		ret = listen(server_fd, 10000);
		if (ret < 0)
		{
			std::cout << "Server  Error Check Port and Adress" << std::endl;
			exit(0);
		}
	}
}


std::vector<unsigned int>	&Server::get_socket_fd(void)
{
	return _socket_fd;
}

std::string					&Server::get_host(void)
{
	return _host;
}
std::vector<location>		&Server::get_locations(void)
{
	return _locations;
}
std::string					&Server::get_server_name(void)
{
	return _server_name;
}
std::vector<unsigned int>			&Server::get_ports(void)
{
	return _ports;
}
std::string				&Server::get_error_pages(void)
{
	return _error_pages;
}
size_t						&Server::get_client_body_size(void)
{
	return _client_body_size;
}
