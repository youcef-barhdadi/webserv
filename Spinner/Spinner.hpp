#ifndef SPINNER_HPP
# define SPINNER_HPP

# include <iostream>
#include <fstream>
# include <string>
# include "../Server/Server.hpp"
#include "../Http/Request.hpp"
#include "../Http/Response.hpp"
#include <unistd.h>
#include <signal.h>
#include <assert.h>
# include "../Connection/Connection.hpp"

class Spinner
{

	public:

		Spinner ();
		~Spinner ();

		Spinner  &		operator=( Spinner  const & rhs );

		void	run();

		void	construct_connections(void);

		std::vector<Server *> _servers;

	private:
		int		accepet(int);
		std::map<unsigned long,  Connection *> ClientFd_Connection;
		Request 		*read_request(int connection_fd);
		Response *getResponse(int connection_fd);

		std::map<unsigned long , Request*>  _requests;

		void		write_responce(int connection_fd);
		std::map<unsigned long, Connection *> serverMap;

		void		init_Spinner();


		unsigned	int _maxfd;

		std::vector<unsigned int> listOfFd;

		std::map<unsigned long , Response*>  _responces;

		void		print_ports();

		std::vector<Connection *> _connections;

		fd_set set_read;
        fd_set set_write;
		time_t	begin;

		Connection *getConnection(std::string host, int port);
		Connection *getConnection(unsigned int  fd);

};


#endif /* ******************************************************** SPINNER _H */
