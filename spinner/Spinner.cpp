/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spinner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:38:39 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/15 22:42:11 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spinner.hpp"
#include <sys/select.h>
#include <algorithm>
#include <map>
#include "../FileDescriptorManager/FileDescriptorManager.hpp"
#include <queue>          // std::queue
#include <fcntl.h>
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


/**
 * 	
 * @brief 
 *
 *  10050 			150 % 10 == 
 */


size_t		getsize(size_t size)
{
	if (1000000  < size )
		return 1000000;
	return size;
}






void	Spinner::run()
{
	struct sockaddr_in address;
    socklen_t addrlen;
    size_t  readlen;
	char buffer[30000] = {0};

	std::map<unsigned long , Request*>  unfinshed_request;
	std::map<unsigned long , Response*>  unfinshed_responce;
	std::queue<Response>     responce_queue;
	fd_set  current_socket, ready_socket;
	fd_set   write_socket;
	FD_ZERO(&current_socket);
	FD_ZERO(&write_socket);
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
		current_socket =write_socket;

			std::cout << "strat " << std::endl;
		if (select((int)maxfd +1, &ready_socket, &current_socket, NULL, NULL) < 0)
		{
			assert(true);
			perror("select error");
			exit(0);
		}

		for (size_t connection_fd = 0; connection_fd < maxfd + 1; connection_fd++)
		{

			if (FD_ISSET(connection_fd, &ready_socket)  || FD_ISSET(connection_fd, &current_socket))
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
					FileDescriptorManager::ADD(new_socket);
					maxfd = std::max(maxfd, (unsigned int)  new_socket);
				}
				else
				{

						std::map<unsigned long , Request*>::iterator iterReq = unfinshed_request.find(connection_fd);
						std::string copy;
						if (iterReq == unfinshed_request.end())
						{
							if (FD_ISSET(connection_fd, &ready_socket))
							{
								readlen = read(connection_fd, buffer, 30000);
								buffer[readlen] = 0;	
								copy = std::string(buffer);
								Request *request = new Request();
								request->Append(copy);
								unfinshed_request.insert(std::make_pair(connection_fd, request));
								FD_SET(connection_fd, &write_socket);
								FileDescriptorManager::REMOVE(connection_fd);
								continue ;  // one read or write per cycle
							}
						}
						if (FD_ISSET(connection_fd, &write_socket))
						{
							std::map<unsigned long, Response *>::iterator iter = unfinshed_responce.find(connection_fd);
							Response *res;
							if (iter == unfinshed_responce.end())
							{
								std::map<unsigned long, Request *>::iterator iter = unfinshed_request.find(connection_fd);
								res  =  new Response(iter->second);
								unfinshed_responce.insert(std::make_pair(connection_fd, res));
							}
							else 
							{
								res = iter->second;
							}

							std::vector<char> array  = res->serv();			
							char *data  = array.data();
							signal(SIGPIPE, SIG_IGN);
							res->bytes_sent += write(connection_fd, data + res->bytes_sent ,getsize(array.size() - res->bytes_sent));
							signal(SIGPIPE, SIG_DFL);
				
							if (res->bytes_sent == array.size())
							{
								std::cout << "end" << std::endl;
								unfinshed_responce.erase(connection_fd);
								unfinshed_request.erase(connection_fd);
								FD_CLR(connection_fd, &write_socket);
							}
						}
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