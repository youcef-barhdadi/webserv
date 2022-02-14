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


std::string		Cgi::startCgi(Request &request)
{	
	
		int pip[2];

	pipe(pip);

	pid_t pid = fork();
	int new_fd;


	// int fd = open("test÷.test", O_CREAT | O_RDWR, 0644);
	// if (fd < 0)
	// {
	// 	perror("rr");
	// 	exit(1);
	// }

	if (pid == 0)
	{
		std::string new_stg = request.get_path().erase(0,1);
		std::cout <<new_stg << std::endl;
		const char *s =new_stg.c_str();
		// dup2(fd, 1);
		dup2(pip[1], 1);
		perror("dup2");
		close(pip[1]);
		perror("close 1");

		close(pip[0]);
		perror("close 2");
		const char *args[] = {"perl", s, NULL };
		// set environment variables
		 execvp("perl", (char **) args);
			perror("perl 1");
			exit(1);

	}
		waitpid(-1, NULL,0);
		char c[500];
		std::cout<< "sssss"<< std::endl;
		close(pip[1]);
		int ret;
		std::string str;
		while ((ret = read(pip[0], c, 499)))
		{
				c[ret] = 0;

				std::string bu(c, c+ret);
				str += c;
	
		}
		std::cout << str<< std::endl;
		return str;


}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */