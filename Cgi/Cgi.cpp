#include "Cgi.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/wait.h>
#include "../Utilities/Utilities.hpp"
#include <signal.h>
#include <stdio.h>
extern char **environ;

Cgi::Cgi()
{
	this->IsTimeOut = false;
}


Cgi::~Cgi()
{
}


std::vector<std::string> parser_cgi_response(std::string str)
{
	std::stringstream ss(str);
	std::string buffer;

	std::string headers;
	std::string body;

	std::vector<std::string> ret;

	while (std::getline(ss, buffer))
	{
		if (buffer == "\r")
		{
			headers.append(buffer+"\n");
				break ;
		}
		headers.append(buffer + "\n");
	}

	while (std::getline(ss, buffer))
	{
		body.append(buffer +"\n");
	}
	ret.push_back(headers);
	ret.push_back(body);
	return ret;
}	


std::vector<char> handlCgiresponse(std::string  str)
{
	
	std::vector<std::string> strs = parser_cgi_response(str);;
	std::cout << strs.size() << std::endl;
	int size = strs[1].length();
	std::string content = "HTTP/1.1 200 OK\nContent-Length: "+ std::to_string(size);
	content += "\n";
	content.append(strs[0]);
	// content.append("\n\r\n");
	content.append(strs[1]);

	std::cout << "[" <<  content << "]" <<  std::endl;
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
	std::string str;

	while ((size =  read(this->pip[0], buffer, 49999)) > 0)
	{
			buffer[size] = 0;
			
			str += std::string(buffer);
			std::cout<< str  << size  << std::endl;
			break ;
	}
	if (size == -1)
	{
		str = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\n";
		str += "Content-Length: 128\n\r\n<html><head><title>500  Internal Server Error</title></head><body><center><h1>Internal Server Error</h1></center></body></html>"; 
	}
	std::vector<char>  vec =  handlCgiresponse(str);
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
// /https://www.oreilly.com/openbook/cgi/ch04_02.html
std::string		Cgi::startCgi(Request *request,  location location)
{
	pipe(this->pip);
	int status = 0;
	int fd;

	fd = -1;
	std::string query_string;
	std::string new_stg = request->get_path();
	std::string	script = location.root  +  new_stg;


	// if (file_exist(script.c_str()) == false)
	// {	
	// 		std::cout << "File dosn't exist" << std::endl;
	// 		this->NotExist = false;	
	// 		return "";
	// }

	if (request->get_method() == "POST")
	{
			fd = open(request->get_body_filename().c_str(), O_RDONLY);
	}
	std::string extention =	getExtension(request->get_path());
	query_string = generate_query_string(request->get_query_parnms());
	std::string  type;


	if (extention == "py")
		type = "python";
	else if (extention == "pl")
		type  = "perl";
	else
		type = "php";

	pid_t worker_pid = fork();
	time_t	begin = time(NULL);
	if (worker_pid == 0)
	{
		if (fd != -1)
			dup2(fd, 0);
		else
		{
			dup2(pip[0], 0);
			close(0);
		}
		dup2(pip[1], 1);
		close(pip[1]);
		close(pip[0]);
		const char *args[] = {"./cgi_tester", "/Users/ybarhdad/Desktop/webserv/var/www/Cgi/file.bla", NULL };
		setenv("QUERY_STRING", query_string.c_str(), 1);
		setenv("REQUEST_METHOD", request->get_method().c_str(), 1);
		// setenv("REQUEST_METHOD", "GET", 1);
		setenv("SERVER_PORT", std::to_string(request->_server->get_ports()[0]).c_str(), 1);
		setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
		
		setenv("PATH_INFO",get_path_info(request->get_method()).c_str(), 1);

		dprintf(2,"\nPATH INFO IS :[%s]\n",get_path_info(request->get_path()).c_str());
		execve("./cgi_tester", (char **) args, environ);

	}

	bool timout(true);
	while (difftime(time(NULL), begin) <= 5)
	{
		int ret = waitpid(worker_pid, &status, WNOHANG);

		if (ret == worker_pid)
		{
			timout = false;
			break;
		}
	}

// forces a fast execution to this time. which is bad
// sleepcp(40000);
	if (timout)
	{
		kill(9,worker_pid);
		this->IsTimeOut = true;
	}
	delete_file(request->get_body_filename());
	return "";
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */


// <html>
// <head>
// <title>Hello - Second CGI Program</title>
// </head>
// <body>
// <h2>Hello None None</h2>
// </body>
// </html>
