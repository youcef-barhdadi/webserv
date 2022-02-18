//
//  Server.hpp  7 Feb 2020
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>

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
		cgi.first = 0;
		upload.first = 0;
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

		void	debug(void);
    // private:
		std::string			_server_name;
		std::string _host;
		std::vector<int>		_port;
		std::vector<error_pages> _error_pages;
		size_t	_client_body_size;
		std::vector<location> _locations;
};