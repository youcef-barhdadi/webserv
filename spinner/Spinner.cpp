/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spinner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:38:39 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/18 01:06:21 by ybarhdad         ###   ########.fr       */
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






void		parseKeepAlive(std::string str)
{
	
}


void	Spinner::run()
{
	struct sockaddr_in address;
    socklen_t addrlen;
    size_t  readlen;
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
		for(size_t j = 0; j < this->_servers[i]->socket_fd.size(); j++)
		{
			FileDescriptorManager::ADD(this->_servers[i]->socket_fd[j]);
			serverMap.insert(std::make_pair(this->_servers[i]->socket_fd[j], this->_servers[i]));
			listOfFd.push_back(this->_servers[i]->socket_fd[j]);
			maxfd = std::max(maxfd, this->_servers[i]->socket_fd[j]);
			
		}		
	}

	std::cout << "size of " << listOfFd.size() << std::endl;
	while (true)
	{
		   timeout.tv_sec  =10;
   			timeout.tv_usec = 0;

		ready_socket = FileDescriptorManager::set;
		current_socket = write_socket;
		if (select((int)maxfd +1, &ready_socket, &current_socket, NULL, NULL) < 0)
		{
			assert(true);
			perror("select error");
			exit(0);
		}

		std::cout << "select return " << std::endl;
		for (size_t connection_fd = 0; connection_fd < maxfd + 1; connection_fd++)
		{
					
			if (FD_ISSET(connection_fd, &ready_socket)  || FD_ISSET(connection_fd, &current_socket))
			{
				// this new connection
				if (std::count(listOfFd.begin(), listOfFd.end() , connection_fd) )
				{
					int new_socket = accept(connection_fd , (struct sockaddr *)&address, (socklen_t*)&addrlen);
					
					fcntl(new_socket, F_SETFL, O_NONBLOCK);
					if (new_socket < 0)
					{
						perror("in accrpt");
						exit(0);
					}
					socketfd_connectionfd.insert(std::make_pair(new_socket, connection_fd));
					FileDescriptorManager::ADD(new_socket);
					maxfd = std::max(maxfd, (unsigned int)  new_socket);
				}
				else
				{
						std::cout << "connection " << connection_fd << std::endl;
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
									unfinshed_request.insert(std::make_pair(connection_fd, request));

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
							int writing = 0;
							errno = 0;
							signal(SIGPIPE, SIG_IGN);

							writing= write(connection_fd, data + res->bytes_sent ,getsize(array.size() - res->bytes_sent));
							signal(SIGPIPE, SIG_DFL);
							std::cout <<connection_fd << std::endl;
							perror("hello ");
							if ( writing == 0 || writing == -1)
							{
								close(connection_fd);
								close(connection_fd);
								FD_CLR(connection_fd, &write_socket);
								unfinshed_responce.erase(connection_fd);
								FileDescriptorManager::REMOVE(connection_fd);
	
								unfinshed_request.erase(connection_fd);
							}
							
							 std::cout << "=="  << writing << std::endl;
							res->bytes_sent += writing;
				
							if (res->bytes_sent == array.size())
							{
								unfinshed_responce.erase(connection_fd);
								unfinshed_request.erase(connection_fd);
								if (res->request->HasHeader("Connection", "keep-alive") == false)
								{
									FD_CLR(connection_fd, &write_socket);
									FileDescriptorManager::REMOVE(connection_fd);
									close(connection_fd);
								}else {
									std::cout << "Connection Closed "  << std::endl;
									FileDescriptorManager::ADD(connection_fd);
									FD_CLR(connection_fd, &write_socket);
		
								}
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