#include "Response.hpp"

std::vector<char> Response::_403_error()
{
	std::string path =  _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/403.html");

	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 106\n\r\n<html><head><title>403 Forbidden</title></head><body><center><h1>403 Forbidden</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char> Response::_405_error()
{
	std::string path =  _request->_server->get_error_pages();
	std::vector<char> file_vec = getfileRaw(path + "/405.html");
	std::string allowed_methods = "";

	for (size_t i = 0; i < _mylocation->methods.size(); i++){
		allowed_methods += _mylocation->methods.at(i);
		if (i != _mylocation->methods.size() - 1)
			allowed_methods += ", ";
	}

	std::string header = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\nAllow: " + allowed_methods + "\r\n";

	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 125\r\nAllow: " + allowed_methods + "\r\n<html><head><title>405 Method Not Allowed</title></head><body><center><h1>405  Method Not Allowed</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;

}

std::vector<char>	Response::_404_error()
{
	std::string path =  _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/404.html");
	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 106\n\r\n<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char>	Response::_501_error()
{
	std::string path = _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/501.html");
	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 118\n\r\n<html><head><title>501 Not Implemented</title></head><body><center><h1>501 Not Implemented</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char>	Response::_400_error()
{
	std::string path = _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/501.html");
	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 110\n\r\n<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char>	Response::_505_error()
{
	std::string path = _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/501.html");
	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 140\n\r\n<html><head><title>505 HTTP Version Not Supported</title></head><body><center><h1>505 HTTP Version Not Supported</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}

std::vector<char>	Response::_413_error()
{
	std::string path = _request->_server->get_error_pages();
	std::string header = "HTTP/1.1 413 Payload Too Large\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/501.html");
	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 122\n\r\n<html><head><title>413 Payload Too Large</title></head><body><center><h1>413 Payload Too Large</h1></center></body></html>";
		std::string l = header + ll;
		std::vector<char> res_vec(l.begin(), l.end());
		return res_vec;
	}

	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

	std::vector<char> return_vec(header.begin(), header.end());

	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
	return return_vec;
}
