//
//
//

#pragma once

#include "../Server/Server.hpp"
#include "../Utilities/Utilities.hpp"
#include <fstream>

class Config{
	public:
		Config(void);
		~Config(void);

		void		parse(std::string filename);
		void		ext_parse(std::ifstream	&ifs);
		void		server_parse(std::ifstream	&ifs, std::string	&server_name);
		void		ext_server_parse(std::vector<std::string>	&buffer_split, Server *serveur);

		location	location_parse(std::ifstream	&ifs);
		void		ext_location_parse(std::vector<std::string>	&buffer_split, location	*loc);

		void	reformat_buffer(std::string	&buffer);
		bool	continue_read(std::string	&buffer);

		void	c_error(std::string	message);

		std::vector<Server *> &get_servers(void);
	private:
		std::vector<Server *> _servers;
};