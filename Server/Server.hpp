#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../Utilities/Utilities.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include  <algorithm>
#include <iostream>
#include <assert.h>
#include <cstring>
#include <arpa/inet.h>

struct location{
	std::string									url;
	std::vector<std::string>					methods;
	std::string									root;
	std::vector<std::string>					index;
	int											autoindex;
	std::string									upload;
	std::vector<std::string>					cgi;
	std::string									redirect;

	void init(void){
		autoindex = 0;
		redirect = "";
		url = "";
		root = "";
		upload = "";

	}
};


class Server{
    public:
        Server(void);
        ~Server(void);

		void	set_server_name(std::string &server_name);
		void	set_port(std::vector<unsigned int> &port);
		void	set_host(std::string &host);
		void	set_error_pages(std::string	&error_pages);
		void	set_client_body_size(size_t &client_body_size);
		void	set_locations(struct location &location);

		void 	create_server(void);


		std::vector<unsigned int>	&get_socket_fd(void);
		std::string					&get_host(void);
		std::vector<location>		&get_locations(void);
		std::string					&get_server_name(void);
		std::vector<unsigned int>			&get_ports(void);
		std::string					&get_error_pages(void);
		size_t						&get_client_body_size(void);

		void	debug(void);
	private:
		std::vector<location> 	_locations;
		std::string _host;
		std::string				_server_name;
		std::vector<unsigned int>		_ports;
		std::string				 _error_pages;
		size_t					_client_body_size;

		std::vector<unsigned int>	_socket_fd;
};
