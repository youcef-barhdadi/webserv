//
//	server.c 02/02/2022 ztaouil
//

// Programming with TCP/IP sockers
// 	1. Create the socket with socket() system call.
// 	2. Identify the socket with bind().
// 	3. On the server, wait for an incoming connection with listen() system call.
// 	4. Send and  receive messages.
// 	5. Close the socket
//

#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include "../RequestHeaderParser/RequestHeader.hpp"

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
		std::stringstream	requestSS;
		char buffer[30000] = {0};
		valread = read(new_socket, buffer, 30000);
		std::string str = std::string(buffer);
		RequestHeader	requestobj(str);

		// requestobj.get_full_request();
		std::cout << "++-++" << std::endl;
		std::cout << requestobj.get_method() << std::endl;
		std::cout << requestobj.get_path() << std::endl;
		std::cout << requestobj.get_version() << std::endl;
		requestobj.debug_headers();
		std::cout << requestobj.get_raw_body() << std::endl;
		write(new_socket, "<h1>oh</h1>\n", 12);
		close(new_socket);
	}
	return 0;
}
