#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "RequestParser/Request.hpp"

int		main(void)
{
	int	server_fd = 0;
	int	new_socket = 0;
	long valread = 0;
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
	memset((char *)&address, 0, sizeof(address));
	memset(address.sin_zero, 0, 8);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

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
	while (1)
	{
		std::cout << "@@@@@@@@@@@@@@@@ Waiting for new connection @@@@@@@@@@@@@@@@" << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("In accept");
			return 0;
		}
		// fcntl(new_socket, F_SETFL, O_NONBLOCK);
		std::stringstream	requestSS;
        char buffer[1024] = {0};
        valread = read(new_socket, buffer, 1024);
		std::string str(buffer);
		Request	requestobj;


        requestobj.Append(str);
		std::cout << std::string(20, '+') << std::endl;
		std::cout << requestobj.get_method() << std::endl;
		std::cout << requestobj.get_path() << std::endl;
		std::cout << requestobj.get_version() << std::endl;
		requestobj.debug_headers();
		requestobj.debug_query_params();

		write(new_socket, "<h1>oh</h1>\n", 12);
		close(new_socket);
	}
	return 0;
}