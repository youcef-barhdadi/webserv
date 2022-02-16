// # include "./NetWorking/Socket.hpp"
# include "./spinner/Spinner.hpp"
# include "./server/Server.hpp"
# include  "./config/Config.hpp"


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
		std::cout << std::endl;
		servers[i].debug();
        s._servers.push_back(&servers[i]);
		
	}
    s.run();

    return 0;
}
