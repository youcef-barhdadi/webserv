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

class Spinner
{

	public:

		Spinner ();
		~Spinner ();

		Spinner  &		operator=( Spinner  const & rhs );

		void	run();
		std::vector<Server *> _servers;


	private:
		int		accepet(int);
		std::map<unsigned long,  unsigned long > socketfd_connectionfd;
		Request 		*read_request(int connection_fd);
		Response *getResponse(int connection_fd);

		std::map<unsigned long , Request*>  _requests;

		void		write_responce(int connection_fd);
		std::map<unsigned long, Server *> serverMap;

		void		init_Spinner();


		unsigned	int _maxfd;

		std::vector<unsigned int> listOfFd;

		std::map<unsigned long , Response*>  _responces;

		void		print_ports();

};


#endif /* ******************************************************** SPINNER _H */
