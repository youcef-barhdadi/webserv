//
//
//

#include "Config.hpp"

Config::Config(void)
{

}

Config::~Config(void)
{

}

void	Config::Parse(std::string filename)
{
	std::ifstream	ifs;
	std::string		buffer;

	ifs.open(filename.c_str(), std::ifstream::in);
	if (ifs.fail()){
		/* failed to open the file*/
		return;
	}


	while (std::getline(ifs, buffer))
	{
		if (buffer[0] == '#' || !buffer.size())
			continue ;
		if (buffer.size()>2 && buffer[0] == '[' && buffer[buffer.size()-1] == ']'){
jump:
			Server			serverbuff;
			buffer = buffer.substr(1, buffer.size()-2);
			serverbuff.set_server_name(buffer);
			while(std::getline(ifs, buffer))
			{
				if (buffer[0] == '#' || !buffer.size())
					continue ;
				if (buffer[0]=='[' && buffer[buffer.size()-1]==']')
					break ;
				buffer = buffer.substr(0, buffer.find('#'));
				std::vector<std::string> vec = ft::split(buffer, ' ');


				if (vec.size() == 2 && vec[0] == "host"){
					serverbuff.set_host(vec[1]);
				}
				else if (vec.size() == 2 && vec[0] == "port")
				{
					try{
						std::vector<std::string> portvec = ft::split(vec[1], ',');
						std::vector<int> portvec2;
						// int p = std::stoi(vec[1]);
						for(size_t i=0; i<portvec.size(); i++)
							portvec2.push_back(std::stoi(portvec[i]));
						serverbuff.set_port(portvec2);
					}catch(...){
						std::cerr << "config error: port" << std::endl;
					}
				}
				else if (vec.size() == 3 && vec[0] == "error_page")
				{
					struct error_pages err;
					err.root = vec[2];
					std::vector<std::string> vec2 = ft::split(vec[1], ',');
					std::vector<int> vec3;
					for(size_t i=0; i<vec2.size(); i++){
						try{
						vec3.push_back(std::stoi(vec2[i]));
						}catch(...){
							std::cerr << "config error: error_pages" << std::endl;
						}
							
					}
					err.status_codes = vec3;
					serverbuff.set_error_pages(err);
				}
				else if (vec.size() == 2 && vec[0] == "client_body_size")
				{
					try{
						size_t t = std::stoi(vec[1]);
						serverbuff.set_client_body_size(t);
					}catch(...){
						std::cerr << "config error: client_body_size" << std::endl;
					}
				}
				else if (vec.size() == 8 && vec[0] == "location")
				{
					struct location loc;
					loc.init();
					loc.redirect = std::make_pair(0, "");
					loc.url = vec[1];
					loc.root = vec[2];
					std::vector<std::string> indexvec = ft::split(vec[3], ',');
					loc.index = indexvec;
					std::vector<std::string> methodsvec = ft::split(vec[4], ',');
					loc.methods = methodsvec;
					try{
						loc.autoindex = stoi(vec[5]);
					}catch(...){
						std::cerr << "config error: location autoindex" << std::endl;
					}
					std::vector<std::string> cgivec = ft::split(vec[6], ',');
					try{
						int cgii = stoi(cgivec[0]);
						cgivec.erase(cgivec.begin());
						loc.cgi = std::make_pair(cgii, cgivec);
					}catch(...){
						std::cerr << "config error: cgi" << std::endl;
					}
					std::vector<std::string> uploadvec = ft::split(vec[7], ',');
					try{
						int upl = stoi(uploadvec[0]);
						loc.upload = std::make_pair(upl, uploadvec[1]);
					}catch(...){
						std::cerr << "config error: uploadlocation" << std::endl;
					}

					serverbuff.set_locations(loc);
				}
				else if (vec.size() == 3 && vec[0] == "location")
				{
					struct location loc;
					loc.init();
					loc.url = vec[1];
					try{
						std::vector<std::string> redirvec = ft::split(vec[2], ',');
						int n = std::stoi(redirvec[0]);
						loc.redirect = std::make_pair(n, redirvec[1]);
					}catch(...){
						std::cerr << "config error: redirect location" << std::endl;
					}
					// redirect location
					serverbuff.set_locations(loc);
				}
				// std::cout << buffer << "[" << buffer.size() << "]" << std::endl;
			}
			_servers.push_back(serverbuff);
			if (buffer.size() > 2 && buffer[0] == '[' && buffer[buffer.size()-1] == ']')
				goto jump;
			// std::cout << buffer << std::endl;
		}
	}

	ifs.close();
}

std::vector<Server> &Config::get_servers(void)
{
	return _servers;
}
