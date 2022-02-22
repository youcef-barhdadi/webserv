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
#include <iostream>     // std::cout
#include <assert.h>
#include <cstring>

struct error_pages{
	std::string									root;
	std::vector<int>							status_codes;
};

struct location{
	std::vector<std::string>					index;
	std::pair<int, std::string>					redirect;
	std::string									url;
	std::string									root;
	std::vector<std::string>					methods;
	int											autoindex;
	std::pair<int, std::vector<std::string> >	cgi;
	std::pair<int, std::string>					upload;

	void init(void){
		autoindex = 0;
		redirect.first = 0;
		cgi.first =0;
		upload.first=0;
	}
};


class Server{
    public:
        Server(void);
        ~Server(void);

		void	set_server_name(std::string &server_name);
		void	set_port(std::vector<int> &port);
		void	set_host(std::string &host);
		void	set_error_pages(struct error_pages &error_pages);
		void	set_client_body_size(size_t &client_body_size);
		void	set_locations(struct location &location);
		void 	create_server();


		std::vector<unsigned int>	&get_socket_fd(void);
		std::string					&get_host(void);
		std::vector<location>		&get_locations(void);
		std::string					&get_server_name(void);
		std::vector<int>			&get_ports(void);
		std::vector<error_pages>	&get_error_pages(void);
		size_t						&get_client_body_size(void);

		void	debug(void);
	private:
		std::vector<unsigned int>	_socket_fd;
		std::string _host;
		std::vector<location> _locations;
		std::string			_server_name;
		std::vector<int>		_ports;
		std::vector<error_pages> _error_pages;
		size_t	_client_body_size;
};
