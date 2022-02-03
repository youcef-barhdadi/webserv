/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spinner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:38:39 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/03 20:16:34 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spinner.hpp"
#include <sys/select.h>
#include <algorithm>
#include <map>
#include "../FileDescriptorManager/FileDescriptorManager.hpp"
#include <queue>          // std::queue

// #include <pair>


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Spinner ::Spinner ()
{
}

Spinner ::Spinner ( const Spinner  & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Spinner ::~Spinner ()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Spinner  &				Spinner ::operator=( Spinner  const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Spinner  const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

// int max(int a, int b)
// {
// 	if (a > b)
// 		return a;
// 	return b;
// }


void	Spinner::run()
{
	struct sockaddr_in address;
    socklen_t addrlen;
    size_t  readlen;
	char buffer[30000] = {0};

	std::map<unsigned long , Response > connection;

	fd_set  current_socket, ready_socket;
	FD_ZERO(&current_socket);


	FileDescriptorManager::CLEAN();

	
	this->_servers[0]->create_server();
 	unsigned	int maxfd = 0;

	for (size_t i = 0; i < this->_servers[0]->socket_fd.size(); i++)
	{	
			// FD_SET(this->_servers[0]->socket_fd[i], &current_socket);
			FileDescriptorManager::ADD(this->_servers[0]->socket_fd[i]);
			maxfd = std::max(maxfd, this->_servers[0]->socket_fd[i] );
	}
	while (true)
	{	

		ready_socket = FileDescriptorManager::set;
		if (select((int)maxfd +1, &ready_socket, NULL, NULL, NULL) < 0)
		{
			assert(true);
			perror("select error");
			exit(0);
		}

		for (size_t connection_fd = 0; connection_fd < maxfd + 1; connection_fd++)
		{

			if (FD_ISSET(connection_fd, &ready_socket))
			{			
				// this new connection
				if (std::count(this->_servers[0]->socket_fd.begin(), this->_servers[0]->socket_fd.end() , connection_fd) )
				{
					int new_socket = accept(connection_fd , (struct sockaddr *)&address, (socklen_t*)&addrlen);
					if (new_socket < 0)
					{
						perror("in accrpt");
						exit(0);
					}
					// FD_SET(new_socket, &current_socket);
					FileDescriptorManager::ADD(new_socket);
					// this->_servers[0]->clients[connection_fd] = new_socket;
					maxfd = std::max(maxfd, (unsigned int)  new_socket );
				}
				else 
				{

					if (connection.find(connection_fd) != connection.end())
					{
						readlen = read(connection_fd, buffer, 30000);
						buffer[readlen] = 0;
						std::string copy = std::string(buffer);
						Request request(copy, connection_fd);
						Response response(request);
						connection.insert(std::make_pair(connection_fd, response));
					}
					Response &res = connection.find(connection_fd)->second;
					// std::vector<char> array  = res.serv();				
					// char *data  = array.data();
					// write(connection_fd,  data,array.size());
					// close(connection_fd);
					// FD_CLR(connection_fd, &current_socket);
				}
			}
		}
	}
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		for (size_t j = 0; j < this->_servers[i]->socket_fd.size(); j++)
		{
			close(this->_servers[0]->socket_fd[0]);
		}
		
	}
	



}


// void	eventLoop()
// {
// 	std::queue<Request> _queue;


// 	_queue.push()	
// }


/*
	event loop
		incoming con 
			->
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */