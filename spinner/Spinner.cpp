/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spinner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:38:39 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/12 04:54:33 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spinner.hpp"
#include <sys/select.h>
#include <algorithm>
#include <map>
<<<<<<< HEAD
#include <assert.h>
=======
#include "../FileDescriptorManager/FileDescriptorManager.hpp"
#include <queue>          // std::queue
#include <fcntl.h>
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
// #include <pair>

#include "../FileDescriptorManager/FileDescriptorManager.hpp"


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

 #define MB 1e6
size_t		getsize(size_t size)
{
	if (MB  < size )
		return MB;
	return size;
}




void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
} 

void	Spinner::run()
{
	// signal(SIGINT, sig_handler);
	struct sockaddr_in address;
    socklen_t addrlen;
    size_t  readlen;
	char buffer[30000] = {0};

	std::map<unsigned long , Request *>  unfinshed_request;
	std::map<unsigned long , Response *>  unfinshed_responce;
	std::queue<Response>     responce_queue;
	fd_set  current_socket, ready_socket;
	fd_set   write_socket;
	FD_ZERO(&current_socket);
<<<<<<< HEAD
	FileDescriptorManager::Clean();

=======
	FD_ZERO(&write_socket);
	FileDescriptorManager::CLEAN();

	
>>>>>>> 6391303bb264a a78e96bc2b5bd0b764103bdc343
	this->_servers[0]->create_server();
 	unsigned	int maxfd = 0;

	for (size_t i = 0; i < this->_servers[0]->socket_fd.size(); i++)
	{	
			// FD_SET(this->_servers[0]->socket_fd[i], &current_socket);
<<<<<<< HEAD
			FileDescriptorManager::Add(this->_servers[0]->socket_fd[i]);
=======
			FileDescriptorManager::ADD(this->_servers[0]->socket_fd[i]);
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
			maxfd = std::max(maxfd, this->_servers[0]->socket_fd[i] );
	}

	while (true)
	{	

<<<<<<< HEAD
		ready_socket = FileDescriptorManager::fd_set;
		if (select((int)maxfd +1, &ready_socket, NULL, NULL, NULL) < 0)
=======
		ready_socket = FileDescriptorManager::set;
		current_socket =write_socket;

			std::cout << "strat " << std::endl;
		if (select((int)maxfd +1, &ready_socket, &current_socket, NULL, NULL) < 0)
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
		{
			// assert(true);
			perror("select error");
			exit(0);
		}
<<<<<<< HEAD
		// FileDe
		for (size_t connection_fd = 0; connection_fd < maxfd + 1; connection_fd++)
=======

		for (size_t connection_fd = 3; connection_fd < maxfd + 1; connection_fd++)
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
		{

			if (FD_ISSET(connection_fd, &ready_socket)  || FD_ISSET(connection_fd, &current_socket))
			{			
				// this new connection
				std::cout << "connection " << connection_fd << std::endl;
				if (std::count(this->_servers[0]->socket_fd.begin(), this->_servers[0]->socket_fd.end() , connection_fd) )
				{
					std::cout << "accpet " << std::endl;
					int new_socket = accept(connection_fd , (struct sockaddr *)&address, (socklen_t*)&addrlen);
					   fcntl(new_socket, F_SETFL, O_NONBLOCK );

					std::cout << "new connection " << new_socket  << std::endl;
					// exit(0);
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

<<<<<<< HEAD
					if (connection.find(connection_fd) == connection.end())
					{
						readlen = read(connection_fd, buffer, 30000);
						buffer[readlen] = 0;
						std::string copy = std::string(buffer);
						std::cout << "hello" << std::endl;
						Request  *request   =  new Request(copy, connection_fd);
						Response response(request);
						connection.insert(std::make_pair(connection_fd, response));
					}
					Response &res = connection.find(connection_fd)->second;
					res.serv(ready_socket);				
					// char *data  = array.data();
					// write(connection_fd,  data,array.size());
					// close(connection_fd);
					// FD_CLR(connection_fd, &current_socket);
=======
							
		
						std::map<unsigned long , Request*>::iterator iterReq = unfinshed_request.find(connection_fd);
						std::string copy;
						if (iterReq == unfinshed_request.end())
						{
							if (FD_ISSET(connection_fd, &ready_socket))
							{
								readlen = read(connection_fd, buffer, 30000);
								buffer[readlen] = 0;	
								copy = std::string(buffer);
								Request *request = new Request(copy, connection_fd);
								unfinshed_request.insert(std::make_pair(connection_fd, request));
								FD_SET(connection_fd, &write_socket);
								FileDescriptorManager::REMOVE(connection_fd);
							}
						}

					if (FD_ISSET(connection_fd, &current_socket))
					{
										std::map<unsigned long, Response *>::iterator iter = unfinshed_responce.find(connection_fd);
											Response *res;
										if (iter == unfinshed_responce.end())
										{
						
											std::map<unsigned long, Request *>::iterator iter = unfinshed_request.find(connection_fd);
											std::cout <<   "======>" << iter->second->getPath() << std::endl;
											res =  new  Response(*iter->second);
											// res = ress;
											unfinshed_responce.insert(std::make_pair(connection_fd, res));
										}
										else 
										{
											res = iter->second;
										}

								
									std::vector<char> array  = res->serv();			
									char *data  = array.data();
									std::cout << "data to send" << array.size() << std::endl;;
									std::cout << res->bytes_sent << "-----"  << std::endl;
									std::cout << getsize(array.size() - res->bytes_sent)  << "===ens" << std::endl;
									signal(SIGPIPE, SIG_IGN);
									 int ret = write(connection_fd, data + res->bytes_sent ,getsize(array.size() - res->bytes_sent));
									 signal(SIGPIPE, SIG_DFL);
									 if (ret > 0)
									 	res->bytes_sent +=ret;
									perror("dddd");
									std::cout <<   "connect_fd" << connection_fd  <<  "<<======================= data to write " << res->bytes_sent << "array size " <<   array.size() << std::endl;

									//unfinshed_responce.erase(connection_fd);
									//unfinshed_responce.insert(std::make_pair(connection_fd, res));
									
									
									if (res->bytes_sent == array.size())
									{
										std::cout << "end" << std::endl;
										unfinshed_responce.erase(connection_fd);
										unfinshed_request.erase(connection_fd);
										// FileDescriptorManager::REMOVE(connection_fd);
										FD_CLR(connection_fd, &write_socket);
										FD_CLR(connection_fd, &current_socket);
									}
					}
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
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