// # include "./NetWorking/Socket.hpp"
# include "./Spinner/Spinner.hpp"
# include "./Server/Server.hpp"
# include  "./Config/Config.hpp"


#include "FileDescriptorManager/FileDescriptorManager.hpp"
int     main(int ac, char **av)
{


   
	if (ac != 2)
		return 1;
	Config config;
	std::string config_file = std::string(av[1]);

	config.Parse(config_file);


	std::vector<Server> servers = config.get_servers();
    Spinner s;

	for(size_t i = 0; i < servers.size(); i++){
		// servers[i].debug();
        s._servers.push_back(&servers[i]);
		
	}
    s.run();

    return 0;
}
