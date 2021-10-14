#include "Cgi.hpp"

#include <unistd.h>


    #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h> 
	     #include <sys/wait.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
{
}

Cgi::Cgi( const Cgi & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cgi::~Cgi()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Cgi &				Cgi::operator=( Cgi const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Cgi const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


void  Cgi::startCgi(Request &request)
{
	pid_t pid = fork();
	int new_fd;
	int pip[2];


	int fd = open("test.test", O_CREAT | O_RDWR, 0644);
	if (fd < 0)
	{
		perror("rr");
		exit(1);
	}

	if (pid == 0)
{
			std::string new_stg = request.getPath().erase(0,1);
				std::cout <<new_stg << std::endl;
	const char *s =new_stg.c_str();

		dup2(fd, 1);
		// std::cout << s << std::endl;
		const char *args[] = {"perl", s, NULL };
		// set environment variables
		 execvp("perl", (char **) args);
		 std::cout << "======="<< std::endl;
		 exit(0);
	}
	else if (pid > 0)
	{
		std::string s;
		// Take input using cin
		std::cin >> s;
		// Print output
		std::cout <<  "-----------------------"<< s<< std::endl;
		waitpid(-1, NULL,0);

		exit(0);
	}
	else
	{

		printf("error");
		// error
	}


}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */