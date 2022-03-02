# include "./Spinner/Spinner.hpp"
# include "./Server/Server.hpp"
# include  "./Config/Config.hpp"

int     main(int ac, char **av)
{
	Config config;
	Spinner s;
	
	if (ac != 2)
		return 1;
	config.parse(av[1]);
	s._servers = config.get_servers();
	s.run();
	return 0;
}
