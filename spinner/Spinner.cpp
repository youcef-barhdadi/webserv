#include "Spinner.hpp"
#include <sys/select.h>
#include <algorithm>



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

	fd_set  current_socket, ready_socket;
	FD_ZERO(&current_socket);

	// for (size_t i = 0; i <   this->_servers.size(); i++)
	// {
		this->_servers[0]->create_server();

	// }      
  unsigned	int maxfd = 0;

	for (size_t i = 0; i < this->_servers[0]->socket_fd.size(); i++)
	{	
			FD_SET(this->_servers[0]->socket_fd[i], &current_socket);
			std::cout << this->_servers[0]->socket_fd[i] << std::endl;
			maxfd = std::max(maxfd, this->_servers[0]->socket_fd[i] );

		/* code */
	}
	
		std::cout << maxfd << std::endl;


	while (true)
	{	

		ready_socket = current_socket;
		std::cout << "started of the loop " << std::endl;
		if (select((int)maxfd, &ready_socket, NULL, NULL, NULL) < 0)
		{
			perror("select error");
			exit(0);
		}
		std::cout<< "end of select"   << std::endl;

		for (size_t i = 0; i < maxfd; i++)
		{
							std::cout<< "workkkkkk"   << std::endl;

			if (FD_ISSET(i, &ready_socket))
			{			
				
				if (std::count(this->_servers[0]->socket_fd.begin(), this->_servers[0]->socket_fd.end() , i))
				{
					int new_socket = accept(i , (struct sockaddr *)&address, (socklen_t*)&addrlen);
					if (new_socket < 0)
					{
						perror("in accrpt");
						exit(0);
					}
					FD_SET(new_socket, &current_socket);
					this->_servers[0]->clients[i] = new_socket;
					// if (new_socket > maxfd)
					// {
						maxfd = std::max(maxfd, (unsigned int)  new_socket );
					// }
				}
				else {
					std::cout << "enter heere" << std::endl;
					readlen = read(i, buffer, 30000);
					buffer[readlen] = 0;
					std::string copy = std::string(buffer);
					Request *request = new Request(copy);
					Response *response = new Response();
					std::vector<char> array  = response->serv(*request);
					// std::cout << res << std::endl;
					char *data  = array.data();
					write(i,  data,array.size());
					close(i);
					FD_CLR(i, &current_socket);
				}
			}
		}
		

	

	}
	close(this->_servers[0]->socket_fd[0]);



}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */