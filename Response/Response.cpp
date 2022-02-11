// unix network programming page 26
//
//

# include "Response.hpp"

Response::Response(RequestHeader const &req)
: _req(req), _headers(), _body(), _file_flag(0)
{
    // std::cout << "Response::Response" << std::endl;
    if (!_req.IsFinished()){
        /* do something */ 

    }

}

Response::~Response(void)
{
    
}

std::string Response::extract_extension(void)
{
    std::string path = _req.get_path();
	if (path.find('.') == std::string::npos){
		return std::string("");
	}
	std::string ext = path.substr(path.find_last_of('.')+1, path.size());
    return ext;
}

std::string Response::build_response(void)
{
    read_raw_file();
    prepare_headers();

    std::string response;
    response += _status_line;
    
    std::map<std::string, std::string>::iterator it = _headers.begin();

    for(; it != _headers.end(); it++)
    {
        response += it->first + ": " + it->second + "\n";
    }

    // response.insert(response.end() - 1, '\r');
    response += "\r\n";
    response += _body;

    return response;
}

void    Response::prepare_headers(void)
{
    // temp
	std::cout << extract_extension() << std::string(100, '&') << std::endl;
	// std::cout << std::string(MimeTypes::getType(extract_extension().c_str())) << std::string(100, '-') << std::endl;
    
	_status_line += "HTTP/1.1 ";

	if (_file_flag)
		_status_line += " 404 Not Found\n";
	else
		_status_line += " 200 OK\n";

	_headers.insert(std::make_pair("Server", "hexagone42"));
	// std::cout << "[" << MimeTypes::getType(extract_extension().c_str()) "]"<< std::string(60, '*') << std::endl;
    if (_body.size()){
		_headers.insert(std::make_pair("Content-Length", std::to_string(_body.size())));
			if (MimeTypes::getType(extract_extension().c_str()) != 0x0)
				_headers.insert(std::make_pair("Content-Type", std::string(MimeTypes::getType(extract_extension().c_str()))));
	}

}

void  Response::read_raw_file(void)
{
    int fd = 0;
    int ret_read = 0;
    char buffer[1024] = {0};
    std::string   path = "." + _req.get_path();
	std::cout << "Path => " << path << std::endl;
	std::string		filetoretrieve = ft::RandString(10);
	std::string		cmd = "ls -l " + path + " | awk -F ' ' '{print $5}' >" + filetoretrieve;
	std::string		cmd2 = "rm " + filetoretrieve;
	std::string		buff;
	size_t		 file_size = 0;

	system(cmd.c_str());

	std::ifstream ifs;

	ifs.open(filetoretrieve, std::ifstream::in);
	std::getline(ifs, buff);

	try{

		file_size = std::stoi(buff);
	}catch(...){
		std::cout << "hadi" << std::endl;
	}

	system(cmd2.c_str());

	std::cout << "file size " << file_size << std::endl;
    std::cout << path << std::endl;

    if ( (fd = open(path.c_str(), O_RDONLY)) < 0){
        perror("cannot open file");
		_file_flag = 1;
    }


    while( (ret_read = read(fd, buffer, 1024)) > 0)
    {
        std::string buffer2(buffer, ret_read);
        _body += buffer2;
    }
	// std::cerr << "I'm just reading " << _body.size() << std::endl;
    
}
