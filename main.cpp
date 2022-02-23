# include "./Spinner/Spinner.hpp"
# include "./Server/Server.hpp"
# include  "./Config/Config.hpp"
# include "FileDescriptorManager/FileDescriptorManager.hpp"
     #include <signal.h>


	 #include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>

void  proc_exit(int a)
{
	int wstat;
	pid_t pid;

	a = 0;

	while (1) {
		pid = wait3(&wstat, WNOHANG, NULL);
		if (pid == 0 || pid == -1)
			return ;
		printf("Return code: %d\n", wstat);
		printf("PID: %d\n", pid);
	}
			return ;
}

int     main(int ac, char **av)
{
	ac = 0;
	// signal (SIGCHLD, proc_exit);
	Config config;
	Spinner s;

	std::string config_file = std::string(av[1]);
	config.Parse(config_file);
	std::vector<Server> servers = config.get_servers();

	for(size_t i = 0; i < servers.size(); i++){
		// servers[i].debug();
		s._servers.push_back(&servers[i]);
	}
	s.run();
	return 0;
}
