#include "Cgi.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/wait.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
     #include <unistd.h>

Cgi::Cgi()
{
}


Cgi::~Cgi()
{
}

std::vector<char> handlCgiresponse(std::string & str)
{

	std::vector<std::string> strs = split(str, '\n');;

	int size = strs[2].length();


	std::string content = "HTTP/1.1 200 OK\nContent-Length: "+ std::to_string(size);
	content += "\n";
	content.append(str);

	std::vector <char> vec(content.begin(), content.end());


	return vec;
}


std::vector<char>	Cgi::readChunk()
{

		char buffer[50000];
		int size = 0;
		size = read(this->pip[0], buffer, 49999);
		if (size < 0)
		{
			perror("hhhh");
			exit(0);
		}
		buffer[size] = 0;
		std::string str(buffer);
		std::vector<char>  vec=  handlCgiresponse(str);
		return vec;
}	

std::string		Cgi::startCgi(Request *request)
{
	pipe(this->pip);
	pid_t pid = fork();

	if (pid == 0)
	{
		std::string new_stg = request->get_path().erase(0,1);
		const char *s =new_stg.c_str();
		errno =0;
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
	close(this->pip[1]);
	return "";
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */