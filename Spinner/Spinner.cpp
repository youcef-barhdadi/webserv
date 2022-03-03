/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spinner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztaouil <ztaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 01:38:39 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/03/03 06:08:55 by ztaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spinner.hpp"
#include <sys/select.h>
#include <algorithm>
#include <map>
#include <queue>
#include <fcntl.h>
#include <ctime>


Spinner ::Spinner ()
{
		_maxfd = 0;
		begin = time(NULL);
}

Spinner ::~Spinner ()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		for (size_t j = 0; j < this->_servers[i]->get_socket_fd().size(); j++)
		{
			close(this->_servers[i]->get_socket_fd()[j]);
		}
	}
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


int		Spinner::accepet(int connection_fd)
{
	struct sockaddr_in address;
	socklen_t addrlen;
	
	int new_socket = accept(connection_fd , (struct sockaddr *)&address, (socklen_t*)&addrlen);
	// std::cout << "timestamp#" << get_time2(begin) << " [" << new_socket << "] incoming connection accepted" << std::endl; 
	fcntl(new_socket, F_SETFL, O_NONBLOCK);
	if (new_socket < 0)
	{
		perror("in accept");
		exit(0);
	}
	this->socketfd_connectionfd.insert(std::make_pair(new_socket, connection_fd));
	// FileDescriptorManager::ADD(FileDescriptorManager::READ, new_socket);
	FD_SET(new_socket, &this->set_read);
	// _maxfd = std::max(_maxfd, (unsigned int)  new_socket);
	return new_socket;
}


Request 		*Spinner::read_request(int connection_fd)
{
	char buffer[30001];
	unsigned long socket_fd;

	Request *request;
	int readlen = read(connection_fd, buffer, 30000);
	if (readlen == 0 || readlen == -1)
	{
		FD_CLR(connection_fd, &this->set_read);
		std::cout << "[" << get_time2(begin) << "] " <<  "closed connection fd:" << connection_fd << std::endl;
		close(connection_fd);
		return NULL;

	}else
		buffer[readlen] = 0;
	std::string copy = std::string(buffer, readlen);
	if (this->_requests.find(connection_fd) == this->_requests.end())
	{
			request = new Request();
			socket_fd =  socketfd_connectionfd[connection_fd];
			request->set_server(serverMap[socket_fd]);
	}
	else
			request = this->_requests[connection_fd];
	request->Append(copy);
	_requests.insert(std::make_pair(connection_fd, request));
	if (request->IsFinished() == true)
	{
		FD_CLR(connection_fd, &this->set_read);
		FD_SET(connection_fd, &this->set_write);
	}
	return request;
}

Response *Spinner::getResponse(int connection_fd)
{
	Response *res;

	std::map<unsigned long, Response *>::iterator iter = _responces.find(connection_fd);
	if (iter == _responces.end())
	{
		std::map<unsigned long, Request *>::iterator iter = _requests.find(connection_fd);
		res  =  new Response(iter->second);
		_responces.insert(std::make_pair(connection_fd, res));
	}
	else
		res = iter->second;
	return res;
}

void		Spinner::write_responce(int connection_fd)
{
	int writing = 0;
	std::vector<char> array ;
	char *data;
	Response *res;

	res = getResponse(connection_fd);
	array = res->serv();
	
	data  = array.data();
	
	signal(SIGPIPE, SIG_IGN);
	writing = write(connection_fd, data + res->get_bytes_sent() ,getsize(array.size() - res->get_bytes_sent()));
	signal(SIGPIPE, SIG_DFL);

	if ( writing == 0 || writing == -1) 
	{
		std::cout << "[" << get_time2(begin) << "] " <<  "closed connection fd:" << connection_fd << std::endl;
		close(connection_fd);
		_responces.erase(connection_fd);
		FD_CLR(connection_fd, &this->set_write);
		FD_CLR(connection_fd, &this->set_read);
		_requests.erase(connection_fd);
	}
	res->set_bytes_sent(res->get_bytes_sent() + writing);
	if (res->get_bytes_sent() == array.size())
	{
		// std::cout << "Delete" << std::endl;
		_responces.erase(connection_fd);
		_requests.erase(connection_fd);
		if (res->get_request()->HasHeader("Connection", "keep-alive") == false || res->close_connection == true)
		{
			FD_CLR(connection_fd, &this->set_write);
			FD_CLR(connection_fd, &this->set_read);
			socketfd_connectionfd.erase(connection_fd);
			std::cout << "[" << get_time2(begin) << "] " <<  "closed connection fd:" << connection_fd << std::endl;
			close(connection_fd);
		}
		else 
		{	
			FD_SET(connection_fd, &this->set_read);
			FD_CLR(connection_fd, &this->set_write);
		}
		delete res;
	}
}

void		Spinner::init_Spinner()
{
	for(size_t i=0; i < this->_servers.size(); i++)
	{
		this->_servers[i]->create_server();
	}

	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		for(size_t j = 0; j < this->_servers[i]->get_socket_fd().size(); j++)
		{
			// FileDescriptorManager::ADD(FileDescriptorManager::READ, this->_servers[i]->get_socket_fd()[j]);
			int fd = this->_servers[i]->get_socket_fd()[j];
			FD_SET(fd, &this->set_read);
			serverMap.insert(std::make_pair(this->_servers[i]->get_socket_fd()[j], this->_servers[i]));
			listOfFd.push_back(this->_servers[i]->get_socket_fd()[j]);
			_maxfd = std::max(_maxfd, this->_servers[i]->get_socket_fd()[j]);

		}
	}
}



void		Spinner::print_ports()
{
	std::cout << "nb of listener fd: " << listOfFd.size() << std::endl;
	std::cout << "server listening on ports: ";
	for(size_t i = 0; i < _servers.size(); i++){
		for(size_t j = 0; j < _servers[i]->get_ports().size(); j++)
			std::cout << _servers[i]->get_ports()[j] << " ";
	}
	std::cout << std::endl;

}

void	Spinner::run()
{
	struct timeval      timeout;

	std::queue<Response>     responce_queue;
	fd_set  current_socket_write, current_socket_read;
	
	FD_ZERO(&current_socket_write);
	FD_ZERO(&current_socket_read);
	
	FD_ZERO(&this->set_read);
	FD_ZERO(&this->set_write);

	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;

	init_Spinner();
	print_ports();

	while (true)
	{
		current_socket_read = this->set_read;
		current_socket_write = this->set_write;
		if (select((int)_maxfd +1, &current_socket_read, &current_socket_write, NULL, &timeout) < 0)
		{
			assert(true);
			perror("select error");
			exit(0);
		}
		for (size_t connection_fd = 0; connection_fd < _maxfd + 1; connection_fd++)
		{
			
			if (FD_ISSET(connection_fd, &current_socket_read)  || FD_ISSET(connection_fd, &current_socket_write))
			{
				if (std::count(listOfFd.begin(), listOfFd.end() , connection_fd) )
				{
					int new_socket = this->accepet(connection_fd);
					std::cout << "[" << get_time2(begin) << "] " <<  "incoming connection fd:" << new_socket << std::endl;
					_maxfd = std::max(_maxfd, (unsigned int)  new_socket);
				}
				else
				{
						if (FD_ISSET(connection_fd, &current_socket_read)){
							std::cout << "[" << get_time2(begin) << "] " <<  "read ready connection fd:" << connection_fd << std::endl;
						 	read_request(connection_fd);
						}
						else{
							std::cout << "[" << get_time2(begin) << "] " <<  "write ready connection fd:" << connection_fd << std::endl;
							write_responce(connection_fd);
						}
				}
			}
		}
	}

}
