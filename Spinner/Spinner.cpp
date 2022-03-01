/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spinner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:38:39 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/03/01 23:07:56 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spinner.hpp"
#include <sys/select.h>
#include <algorithm>
#include <map>
#include "../FileDescriptorManager/FileDescriptorManager.hpp"
#include <queue>
#include <fcntl.h>
#include <ctime>


Spinner ::Spinner ()
{
}

Spinner ::~Spinner ()
{
}


/**
 *  10050 			150 % 10 ==
 */


size_t		getsize(size_t size)
{
	if (1000000  < size )
		return 1000000;
	return size;
}


void		parseKeepAlive(std::string str)
{
	(void)str;
}

// lost pointers.
void	Spinner::run()
{
	time_t	begin = time(NULL);
	struct sockaddr_in address;
	socklen_t addrlen;
	int  readlen;
	char buffer[30000] = {0};
	struct timeval      timeout;

	std::map<unsigned long , Request*>  unfinshed_request;
	std::map<unsigned long , Response*>  unfinshed_responce;
	std::queue<Response>     responce_queue;
	fd_set  current_socket, ready_socket;
	fd_set   write_socket;
	FD_ZERO(&current_socket);
	FD_ZERO(&write_socket);
	FileDescriptorManager::CLEAN();

	std::map<unsigned long, Server *> serverMap;
	std::map<unsigned long,  unsigned long > socketfd_connectionfd;



	for(size_t i=0; i < this->_servers.size(); i++)
	{
		this->_servers[i]->create_server();
	}
	unsigned	int maxfd = 0;
	std::vector<unsigned int> listOfFd;
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		for(size_t j = 0; j < this->_servers[i]->get_socket_fd().size(); j++)
		{
			FileDescriptorManager::ADD(this->_servers[i]->get_socket_fd()[j]);
			serverMap.insert(std::make_pair(this->_servers[i]->get_socket_fd()[j], this->_servers[i]));
			listOfFd.push_back(this->_servers[i]->get_socket_fd()[j]);
			maxfd = std::max(maxfd, this->_servers[i]->get_socket_fd()[j]);

		}
	}

	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;
	std::cout << "nb of listener fd: " << listOfFd.size() << std::endl;
	/* printing to stdout the ports the server listening on*/
	std::cout << "server listening on ports: ";
	for(size_t i = 0; i < _servers.size(); i++){
		for(size_t j = 0; j < _servers[i]->get_ports().size(); j++)
			std::cout << _servers[i]->get_ports()[j] << " ";
	}
	std::cout << std::endl;
	while (true)
	{
		// std::cout << std::string(15, '*') << "mainloop-begin" << std::string(15, '*') << std::endl;
		ready_socket = FileDescriptorManager::set;
		current_socket = write_socket;
		if (select((int)maxfd +1, &ready_socket, &current_socket, NULL, &timeout) < 0)
		{
			assert(true);
			perror("select error");
			exit(0);
		}

		// std::cout << "select returns" << std::endl;
		for (size_t connection_fd = 0; connection_fd < maxfd + 1; connection_fd++)
		{
			
			if (FD_ISSET(connection_fd, &ready_socket)  || FD_ISSET(connection_fd, &current_socket))
			{
				// this new connection
				if (std::count(listOfFd.begin(), listOfFd.end() , connection_fd) )
				{
					int new_socket = accept(connection_fd , (struct sockaddr *)&address, (socklen_t*)&addrlen);
					std::cout << "timestamp#" << get_time2(begin) << " [" << new_socket << "] incoming connection accepted" << std::endl; 
					fcntl(new_socket, F_SETFL, O_NONBLOCK);
					if (new_socket < 0)
					{
						perror("in accept");
						exit(0);
					}
					socketfd_connectionfd.insert(std::make_pair(new_socket, connection_fd));
					FileDescriptorManager::ADD(new_socket);
					maxfd = std::max(maxfd, (unsigned int)  new_socket);
				}
				else
				{
					std::cout << "timestamp#" << get_time2(begin) << " [" << connection_fd << "] connection ready for i/o" << std::endl;
					std::map<unsigned long , Request*>::iterator iterReq = unfinshed_request.find(connection_fd);
					std::string copy;
					if (iterReq == unfinshed_request.end())
					{
						if (FD_ISSET(connection_fd, &ready_socket))
						{
							readlen = read(connection_fd, buffer, 30000);
							if (readlen == 0 || readlen == -1)
							{
								close(connection_fd);
								FileDescriptorManager::REMOVE(connection_fd);
								continue ;
							}else
								buffer[readlen] = 0;
							copy = std::string(buffer);
							Request *request = new Request();
							//get the server
							unsigned long socket_fd =  socketfd_connectionfd[connection_fd];
							request->set_server(serverMap[socket_fd]);

							request->Append(copy);
							if (request->IsFinished() == true)
							{
								unfinshed_request.insert(std::make_pair(connection_fd, request));
								FD_SET(connection_fd, &write_socket);
								FileDescriptorManager::REMOVE(connection_fd);
							}else
							{
								unfinshed_request.insert(std::make_pair(connection_fd, request));
							}

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
						// std::cerr << std::string(50, '-') << std::endl;
						// std::cerr << data << std::endl;
						// std::cerr << std::string(50, '-') << std::endl;
						int writing = 0;
						errno = 0;
						signal(SIGPIPE, SIG_IGN);

						writing = write(connection_fd, data + res->get_bytes_sent() ,getsize(array.size() - res->get_bytes_sent()));
						signal(SIGPIPE, SIG_DFL);
						if ( writing == 0 || writing == -1)
						{
							// std::map<unsigned long , Request*>::iterator it1 = unfinshed_request.find(connection_fd);
							// std::map<unsigned long , Response*>::iterator it2 = unfinshed_responce.find(connection_fd);

							close(connection_fd);
							close(connection_fd);
							FD_CLR(connection_fd, &write_socket);

							// delete it1->second;
							// delete it2->second;
							unfinshed_responce.erase(connection_fd);
							FileDescriptorManager::REMOVE(connection_fd);
							unfinshed_request.erase(connection_fd);

						}

						//  std::cout << "=="  << writing << std::endl;
						res->set_bytes_sent(res->get_bytes_sent() + writing);

						if (res->get_bytes_sent() == array.size())
						{
							// std::map<unsigned long , Request*>::iterator it1 = unfinshed_request.find(connection_fd);
							// std::map<unsigned long , Response*>::iterator it2 = unfinshed_responce.find(connection_fd);

							unfinshed_responce.erase(connection_fd);
							unfinshed_request.erase(connection_fd);
							if (res->get_request()->HasHeader("Connection", "keep-alive") == false || res->close_connection == true)
							{
								FD_CLR(connection_fd, &write_socket);
								FileDescriptorManager::REMOVE(connection_fd);
								socketfd_connectionfd.erase(connection_fd);
								close(connection_fd);
								std::cout << "timestamp#" << get_time2(begin) << " [" << connection_fd  << "] connection closed" << std::endl;
								// delete it1->second;
								// delete it2->second;
							}
							else 
							{
								FileDescriptorManager::ADD(connection_fd);
								FD_CLR(connection_fd, &write_socket);
							}
							delete res;
						}
					}
				}
			}
		}
	}
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		for (size_t j = 0; j < this->_servers[i]->get_socket_fd().size(); j++)
		{
			close(this->_servers[i]->get_socket_fd()[j]);
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
