//
//  Request.cpp   7 feb 2020  ztaouil
//

#include "Request.hpp"

Request::Request(void)
: _buffer(), _method(), _path(), _protocol_version(0), _body_filename(), _body_size(0)
, _isFinished(false), _isHeaderParsed(false), _debug(0)
{

}

Request::~Request(void)
{

}

Request::Request(Request const &rhs)
: _buffer(), _method(), _path(), _protocol_version(0), _body_filename(), _body_size(0)
, _isFinished(false), _isHeaderParsed(false)
{
    // std::cout << "Request::Request(Request const &rhs)" << std::endl;
    *this = rhs;

}

Request   &Request::operator=(Request const &rhs)
{
    // std::cout << "Request::operator=" << std::endl;
    if (this != &rhs){
        _buffer = rhs._buffer;
        _method = rhs._method;
        _path = rhs._path;
        _protocol_version = rhs._protocol_version;
        _headers = rhs._headers;
        _query_params = rhs._query_params;
        _body_filename = rhs._body_filename;
        _body_size = rhs._body_size;
        _isFinished = rhs._isFinished;
        _isHeaderParsed = rhs._isHeaderParsed;
    }

    _debug += 1;
    return *this;
}

void    Request::Append(std::string &Message)
{
    _buffer += Message;

    if (_isHeaderParsed == false && HeadersFinished()){
        ParseHeaders();
        ParseQueryParams();
        _isHeaderParsed = true;
    }

    if (_isHeaderParsed && _headers.find("Content-Length") != _headers.end() && std::stoi(_headers["Content-Length"]) > 0)
    {
        _body_filename = RandString(30);
        std::ofstream ofs (_body_filename, std::ofstream::out);
        if (_headers["Transfer-Encoding"] == "chunked")
        {
            // I am supposing that the second and nth request contains only the body.

            std::stringstream ss(_buffer);
            std::string buff;

            std::getline(ss, buff);
            size_t n = HexToDec(buff);
            if (buff == "0\r")
                _isFinished = true;
            while (std::getline(ss, buff) && n > _body_size)
            {
                ofs << ss;
                n += buff.size();
                if (n > _body_size)
                    ofs << "\n";
            }
        }else{
            ofs << _buffer;
            _body_size += _buffer.size();
            std::cout << "==>" << _body_size << std::endl;
        }
        _buffer.clear();
        ofs.close();
    }
    else if (_headers["Transfer-Encoding"] == "chunked" && BodyFinished())
        _isFinished = true;

    if (_isFinished)
    {
        VerifyRequest();
    }
}

bool    Request::HeadersFinished(void)
{
    std::stringstream ss(_buffer);
    std::string buffer;
    bool cond(false);

    while (std::getline(ss, buffer))
    {
        if (buffer == "\r"){
            cond = true;
            break;
        }
    }
    return cond;
}

bool    Request::BodyFinished(void)
{
    size_t content_length = 0;
    if (_headers.find("Content-Length") != _headers.end())
        content_length = std::stoi(_headers["Content-Length"]);        

    return content_length <= _body_size;
}

bool     Request::IsFinished(void)
{
    return _isFinished;
}

void       Request::ParseHeaders(void)
{
    std::stringstream   ss(_buffer);

    std::string buffer;
    std::getline(ss, buffer);
    std::vector<std::string> firstline = split(buffer, ' ');

    if (firstline.size() != 3)
        throw RequestError();

    _method = firstline[0];
	std::cout << _method << "]]]]\n";
    _path = firstline[1];
    _protocol_version = stof(split(firstline[2], '/')[1]);

    if (_path == "/")
        _path = "/index.html";

    while (std::getline(ss, buffer))
    {
        if (buffer == "\r")
            break;
        std::vector<std::string> myvec = split(buffer, ':');
        _headers.insert(std::make_pair(trim(myvec[0]), trim(myvec[1])));
    }

    _buffer.clear();
    while (std::getline(ss, buffer))
    {
        _buffer += buffer + "\n";
    }
    if (!_buffer.empty())
        _buffer.erase(_buffer.end() - 1);
}

void    Request::VerifyRequest(void)
{
    // std::cout << "Request::ParseVerify" << std::endl;
    std::string methods[3] = {"GET", "POST", "DELETE"};
    int found = 0;

    for(int i = 0; i < static_cast<int>(sizeof(methods)/sizeof(std::string)); i++)
        if (_method == methods[i])
            found = 1;
    
    if (!found)
		throw RequestError();
}

void    Request::ParseQueryParams(void)
{
    if (_path.find('?') != std::string::npos)
    {
        std::vector<std::string> myvec1 = split(_path, '?');
        _path = myvec1[0];
        std::vector<std::string> myvec2 = split(myvec1[1], '&');
        size_t n = std::count(myvec1[1].begin(), myvec1[1].end(), '&');
        if (myvec2.size() != n + 1){
            throw BadRequest();
        }
        for(size_t i = 0; i < myvec2.size(); i++)
        {
            std::vector<std::string> myvec3 = split(myvec2[i], '=');
            _query_params.insert(std::make_pair(myvec3[0], myvec3[1])); 
        }
    }
}

bool    Request::QueryParamsEmpty(void)
{
    if (!_query_params.size())
        return true;
    return false;
}

void   Request::debug_query_params(void)
{
    std::cout << std::string(20, '-') << "+QUERYPARAMS+" << std::string(20, '-') << std::endl;
    std::map<std::string, std::string>::iterator it = _query_params.begin();
    for(; it != _query_params.end(); it++){
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

std::string Request::get_method(void) const
{
    return _method;
}

std::string Request::get_path(void) const
{
    return _path;
}

float Request::get_version(void)
{
    return _protocol_version;
}

void    Request::debug_headers(void)
{
    std::map<std::string, std::string>::iterator it = _headers.begin();

    for(; it != _headers.end(); it++){
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void    Request::get_buffer(void)
{
    std::cout << _buffer << std::endl;
}


void		Request::set_path(std::string path)
{
		this->_path = path;
}

std::string	Request::get_body_filename(void)
{
	return _body_filename;
}
