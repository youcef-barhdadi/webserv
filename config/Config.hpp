//
//
//

#pragma once

#include "../Server/Server.hpp"
#include "../utilities/utilities.hpp"
#include <fstream>

class Config{
	public:
		Config(void);
		~Config(void);

		void	Parse(std::string filename);

		std::vector<Server> &get_servers(void);
	private:
		std::vector<Server> _servers;
};