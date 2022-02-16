//
//  Server.cpp  7 Feb 2020
//

#include "Server.hpp"

Server::Server(void)
: _client_body_size(0)
{

}

Server::~Server(void)
{

}

void	Server::set_server_name(std::string &server_name){
	this->_server_name = server_name;
}
void	Server::set_port(std::vector<int> &port){
	this->_ports = port;
}
void	Server::set_host(std::string &host){
	this->_host = host;
}
void	Server::set_error_pages(struct error_pages &error_pages){
	this->_error_pages.push_back(error_pages);
}
void	Server::set_client_body_size(size_t &client_body_size){
	this->_client_body_size = client_body_size;
}
void	Server::set_locations(struct location &location){
	this->_locations.push_back(location);
}


void	Server::debug(void)
{
	std::cout << std::string(50, '&') << std::endl;
	std::cout << "server_name: " << _server_name << std::endl;
	std::cout << "host: " << _host << std::endl;
	std::cout << "port: "; for(size_t i=0; i<_ports.size(); i++){std::cout << _ports[i] << " ";}; std::cout << '\n';
	std::cout << "error_pages: " << std::endl;
	for(size_t i=0;i<_error_pages.size();i++){
		std::cout << std::string(30,'+') << std::endl;
		std::cout << "\troot="<<_error_pages[i].root<<", ";
		for(size_t j=0;j<_error_pages[i].status_codes.size();j++){
			std::cout << _error_pages[i].status_codes[j] << " ";
		}
		std::cout << "\\" << std::endl;
		std::cout << std::string(30, '+') << std::endl;
	}
	std::cout << "client_body_size: " << _client_body_size << std::endl;
	std::cout << "locations: " << std::endl;
	for (size_t i=0; i<_locations.size(); i++)
	{
		std::cout << std::string(30, '+') << std::endl;
		std::cout << "\t" << "url: " << _locations[i].url << std::endl;
		std::cout << "\t" << "root: " << _locations[i].root << std::endl;
		std::cout << "\t" << "methods: ";for(size_t j=0;j<_locations[i].methods.size(); j++){std::cout <<_locations[i].methods[j] <<" "; };std::cout << "\n";
		std::cout << "\t" << "autoindex: " << _locations[i].autoindex << std::endl;
		std::cout << "\t" << "cgi: " << _locations[i].cgi.first<<" ";for(size_t j=0;j<_locations[i].cgi.second.size();j++){std::cout << _locations[i].cgi.second[j] << " ";};std::cout <<"\n";
		std::cout << "\t" << "index: ";for(size_t j=0; j<_locations[i].index.size(); j++){std::cout << _locations[i].index[j] << " ";}; std::cout << "\n";
		std::cout << std::string(30, '+') << std::endl;
	}
	std::cout << std::string(50, '&') << std::endl;
}


void Server::create_server()
{
	for (int i = 0; i <  this->_ports.size(); i++)
	{
int n;
unsigned int m = sizeof(n);
int fdsocket;

		int	server_fd = socket(AF_INET, SOCK_STREAM, 0);

		int enable = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			perror("setsockopt");
		fcntl(server_fd, F_SETFL, O_NONBLOCK);
		assert((server_fd > 0));
		if (server_fd < 0)
		{
			perror("error");
			exit(0);
		}
		this->socket_fd.push_back(server_fd);
		struct  sockaddr_in address;
		memset((char *)&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		//conevert
		address.sin_addr.s_addr= INADDR_ANY;
		address.sin_port =htons(this->_ports[i]);
		int ret =  bind(server_fd, (struct sockaddr *)&address , sizeof(address));
		assert(ret == 0);
		if (ret < 0)
		{
			perror("error ");
			return ;
		}
		 ret = listen(server_fd, 10000);
		assert(ret == 0);
		if (ret < 0)
		{
			perror("error ");
			return ;
		}
	}
}