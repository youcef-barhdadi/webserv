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
		std::cout << "[" << str << "]" << std::endl;
		std::vector<char>  vec=  handlCgiresponse(str);
		return vec;
}	


std::string		Cgi::startCgi(Request *request)
{
	pipe(this->pip);
	int opserver  = fork();
	if (opserver== 0)
	{
		pid_t  pid = fork();

		if (pid == 0)
		{
			pid_t worker_pid = fork();
			if (worker_pid == 0)
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
			pid_t timeout_pid = fork();
			if (timeout_pid == 0)
			{
				sleep(10);
				write(2, "xxxx\n", 5);
				_exit(0);
			}
			pid_t exited_pid = wait(NULL);
			if (exited_pid == worker_pid)
			{
				kill(timeout_pid, SIGKILL);
			}
			else 
			{
				errno = 0;
				std::string header = "HTTP/1.1 504 Gateway Timeout\r\nContent-Type: text/html\n";
				std::string ll = "Content-Length: 117\n\r\n<html><head><title>504 Gateway Timeout</title></head><body><center><h1>504 Gateway Timeout</h1></center></body></html>";
				std::string l = header + ll;
				std::vector<char> res_vec(l.begin(), l.end());
				std::cout << l << std::endl;
				int ret = 	write(pip[1], res_vec.data(), res_vec.size());
				std::cout << ret << "-------------------" << std::endl;
				kill(worker_pid, SIGKILL); 
			}
			wait(NULL); 
			_exit(0); 
		}
		close(this->pip[1]);
		return "";
	}
			return "";

}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */