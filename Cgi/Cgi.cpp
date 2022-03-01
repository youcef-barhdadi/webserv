#include "Cgi.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/wait.h>
#include "../Utilities/Utilities.hpp"
#include <signal.h>
Cgi::Cgi()
{
	this->IsTimeOut = false;
}


Cgi::~Cgi()
{
}



std::vector<char> handlCgiresponse(std::string  str)
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

	if (this->IsTimeOut)
	{
		std::string header = "HTTP/1.1 504 Gateway Timeout\r\nContent-Type: text/html\n";
		std::string ll = "Content-Length: 120\n\r\n<html><head><title>504  Gateway Timeout</title></head><body><center><h1>504  Gateway Timeout</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}
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




void sleepcp(int milliseconds) // Cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}



//https://en.wikipedia.org/wiki/Common_Gateway_Interface
std::string		Cgi::startCgi(Request *request,  location location)
{
	pipe(this->pip);
	int status = 0;
	int fd;


	if (request->get_method() == "POST")
	{
			fd = open(request->get_body_filename().c_str(), O_RDONLY);
	}

	std::string extention =	getExtension(request->get_path());
	std::string  type;
	if (extention == "py")
		type = "python";
	else
		type  = "perl";
	pid_t worker_pid = fork();
	if (worker_pid == 0)
	{
		std::string new_stg = request->get_path().erase(0,1);
		const char *script = (location.root +  new_stg).c_str();
		errno =0;
		if (fd != -1)
		{
			dup2(fd, 0);
		}
		dup2(pip[1], 1);
		close(pip[1]);
		close(pip[0]);
		const char *args[] = {type.c_str(), script, NULL };
		// set environment variables
		execvp(type.c_str(), (char **) args);
	}
	sleepcp(40000);
	int ret = waitpid(worker_pid, &status, WNOHANG);
	if (ret ==0)
	{
		kill(9,worker_pid);
		this->IsTimeOut = true;
	}
	return "";
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */