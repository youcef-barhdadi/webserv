
#pragma once
#include "../Server/Server.hpp"
#include <vector>
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


class Connection{


	public:
				std::string _host;
				unsigned	int			_fd;
				int			_port;
				std::vector<Server *> _servers;
				void add_server(Server *);
				Connection(std::string host, int port);
				void create_server();


};