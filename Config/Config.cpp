//
//
//

#include "Config.hpp"

Config::Config(void)
{

}

Config::~Config(void)
{
	std::vector<Server *>::iterator	it = _servers.begin();

	while (it != _servers.end()){
		delete *it;
		it++;
	}
}

void	Config::parse(std::string filename)
{
	std::ifstream	ifs;

	ifs.open(filename);
	if (ifs.fail())
		c_error("failed to open config file");
	ext_parse(ifs);
	ifs.close();
}

void	Config::ext_parse(std::ifstream	&ifs)
{
	std::string	buffer;

	while (std::getline(ifs, buffer)){
		if (continue_read(buffer))
			continue ;
		reformat_buffer(buffer);

		if (*buffer.begin() == '[' && *(buffer.end() - 1) == ']')
			server_parse(ifs, buffer);

		// //std::cout << buffer << std::endl;
	}
}

void	Config::server_parse(std::ifstream	&ifs, std::string	&server_name)
{
	std::string	buffer;
	Server	*serveur = new Server;

	server_name = server_name.substr(1, server_name.size()-2);
	serveur->set_server_name(server_name);
	// //std::cout << "Config::server_parse" << std::endl;
	while(std::getline(ifs, buffer)){
		if (continue_read(buffer))
			continue;
		reformat_buffer(buffer);
		if (buffer == "[end]")
			break ;
		if (buffer == "location{"){
			location loc = location_parse(ifs);
			serveur->set_locations(loc);
			continue;
		}
		std::vector<std::string>	buffer_split = split(buffer, '=');
		ext_server_parse(buffer_split, serveur);

	}
	_servers.push_back(serveur);
}

void		Config::ext_server_parse(std::vector<std::string>	&buffer_split, Server *serveur)
{
	if (buffer_split.size() != 2)
		c_error(buffer_split[0]);

	if (buffer_split[0] == "host")
		serveur->set_host(buffer_split[1]);
	else if (buffer_split[0] == "port"){
		std::vector<std::string>	vec1 = split(buffer_split[1], ',');
		std::vector<unsigned int>	vec2;
		for (size_t i = 0; i < vec1.size(); i++){
			try{
				vec2.push_back(std::stoi(vec1[i]));
			}catch(...){
				c_error(buffer_split[0]);
			}
		}
		serveur->set_port(vec2);
	}
	else if (buffer_split[0] == "error_pages"){
		serveur->set_error_pages(buffer_split[1]);
	}
	else if (buffer_split[0] == "client_body_size"){
		try{
			size_t	nb = std::stoi(buffer_split[1]);
			serveur->set_client_body_size(nb);
		}catch(...){
			c_error(buffer_split[1]);
		}
	}
	else
		c_error("");

}

location	Config::location_parse(std::ifstream	&ifs)
{
	// //std::cout << "Config::location_parse" << std::endl;
	std::string	buffer;
	location	loc;

	loc.init();
	while (std::getline(ifs, buffer))
	{
		if (continue_read(buffer))
			continue;
		reformat_buffer(buffer);
		if (buffer == "}")
			break;
		std::vector<std::string>	buffer_split = split(buffer, '=');
		ext_location_parse(buffer_split, &loc);
	}
	return loc;
}

void	Config::ext_location_parse(std::vector<std::string>	&buffer_split, location	*loc)
{
	if (buffer_split.size() != 2)
		c_error(buffer_split[0]);
	
	if (buffer_split[0] == "url"){
		loc->url = buffer_split[1];
	}
	else if (buffer_split[0] == "method"){
		loc->methods = split(buffer_split[1], ',');
	}
	else if (buffer_split[0] == "root"){
		loc->root = buffer_split[1];
	}
	else if (buffer_split[0] == "index"){
		loc->index = split(buffer_split[1], ',');
	}
	else if (buffer_split[0] == "autoindex" && buffer_split[1] == "on"){
		loc->autoindex = 1;
	}
	else if (buffer_split[0] == "cgi"){
		loc->cgi = split(buffer_split[1], ',');
	}
	else if (buffer_split[0] == "upload"){
		loc->upload = buffer_split[1];
	}
	else if (buffer_split[0] == "redirect")
		loc->redirect = buffer_split[1];
	else
		c_error("");
}

//	buffer utility
void	Config::reformat_buffer(std::string	&buffer)
{
	buffer = buffer.substr(0, buffer.find('#'));
	buffer = trim(buffer);
}

bool	Config::continue_read(std::string	&buffer)
{
	if (buffer[0] == '#' || !buffer.size())
		return true;
	return false;
}

void	Config::c_error(std::string	message)
{
	std::cout << "config error: " << message << std::endl;
	exit(1);
}

std::vector<Server *> &Config::get_servers(void)
{
	return _servers;
}