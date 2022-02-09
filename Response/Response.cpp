// unix network programming page 26
//
//

# include "Response.hpp"

Response::Response(RequestHeader const &req)
: _req(req), _headers(), _body()
{

}

Response::~Response(void)
{
    
}

void    Response::prepare_headers(void)
{
    // temp
    _headers += "HTML/1.1 200 OK";
}

std::vector<char>   Response::read_raw_file(void)
{
    std::ifstream ifs;
    std::string   path = ".." + _req.get_path();

    std::cout << path << std::endl;
    ifs.open("path", std::ifstream::in);

    ifs.close();
}