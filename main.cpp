//
//	server.c 02/02/2022 ztaouil
//

// Exercices page 64.

// Programming with TCP/IP sockers
// 	1. Create the socket with socket() system call.
// 	2. Identify the socket with bind().
// 	3. On the server, wait for an incoming connection with listen() system call.
// 	4. Send and  receive messages.
// 	5. Close the socket
//

#include <sys/time.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "Request/Request.hpp"
#include "Response/Response.hpp"

int		main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	Config config;
	std::string config_file = std::string(av[1]);

	config.Parse(config_file);

	std::vector<Server> servers = config.get_servers();
	int	server_fd = 0;
	int valread = 0;
	const int PORT = 1339;
	// for ip networking we use struct sockaddr_in
	struct sockaddr_in	address;
	int addrlen = sizeof(address);

	// creating a socket with socket() system call
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("cannot create socket");
		return 0;
	}
	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	address.sin_port = htons(PORT);

	int enable = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		perror("setsockopt");
	// bind() assigns a name to an unamed socket
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		return 0;
	}

	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		return 0;
	}
	// fcntl(server_fd, F_SETFL, O_NONBLOCK);

	// fd_set
	fd_set	current_sockets, ready_sockets;

	FD_ZERO(&current_sockets);
	FD_SET(server_fd, &current_sockets);

	int maxfd = server_fd + 1;
	int client_socket=-1;

	struct timeval	_time;

	_time.tv_sec = 10;
	_time.tv_usec = 0;

	std::cout << "server listening on Port " << PORT << std::endl;
	while (1)
	{
		// because select in destructive
		ready_sockets = current_sockets;

		if (select(maxfd, &ready_sockets, NULL, NULL, &_time) < 0){
			perror("select error");
			exit(1);
		}

		// now ready_sockets are ready... or not

		for (int i=0; i < maxfd; ++i){
			if (FD_ISSET(i, &ready_sockets)){
				if (i == server_fd){
					// this is a new connection
					std::cout << "New connection established fd:[" << i << "]" << std::endl;
					if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
					{
						perror("In accept");
						return 0;
					}
					if (client_socket + 1 > maxfd)
						maxfd = client_socket + 1;
					// std::cout << "=>" << client_socket << std::endl;
					// accept the new connection and add to it the fd set to watch current_sockets
					FD_SET(client_socket, &current_sockets);
				} else{
					std::cout << "Existing Connection is ready for i/o fd:[" << i << "]" << std::endl; 
					// handle connection and remove it from the fd set we're watching
					Request	req;
					
					while (valread != -1){
						char buffer[1024] = {0};
						valread = read(i, buffer, 1024);
						if (valread != -1){
							std::string str(buffer);
							req.Append(str);
						}
					}

					std::cout << std::string(40, '+') << std::endl;
					std::cout << req.get_method() << " ";
					std::cout << req.get_path() << " ";
					std::cout << req.get_version() << std::endl;
					req.debug_headers();
					req.debug_query_params();
					std::cout << std::string(40, '+') << std::endl;

					// sending data
					Response		res(req, servers);

					std::string response = res.build_response();
					std::cout << std::string(40, '-') << std::endl;
					// std::cout << response << std::endl;
					std::cout << "response weights " << response.size() << " bytes" << std::endl;
					// std::cout << response.length() << std::endl;
					// exit(0);

					
					for (size_t j = 0; j < response.size(); j++){
						valread = write (i, &response.c_str()[j], 1);
						if (valread == -1)
							break;
					}

					std::cout << "write returned " << valread << std::endl;
					std::cout << std::string(40, '-') << std::endl;
					// std::cout << (valread == static_cast<int>(res.size())) << std::endl;
					// removing the socket from current sockets then closing the fd
					FD_CLR(i, &current_sockets);
					close(i);
					valread = 0;
					if (i == maxfd){
						while (FD_ISSET(maxfd, &current_sockets) == 0)
							maxfd--;
						maxfd++;
					}
				}
			}
		}

	}

	for (int i = 0; i < maxfd; ++i){
		if (FD_ISSET(i, &current_sockets))
			close(i);
	}

		
	return 0;
}
