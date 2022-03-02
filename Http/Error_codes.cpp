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
	std::string header = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\n";
	std::vector<char> file_vec = getfileRaw(path + "/405.html");
	if (path.size() == 0 || !file_vec.size())
	{
		std::string ll = "Content-Length: 125\n\r\n<html><head><title>405 Method Not Allowed</title></head><body><center><h1>405  Method Not Allowed</h1></center></body></html>";
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

// 400 bad request

// std::vector<char>	Response::_504_error()
// {
// 	std::string path = _request->_server->get_error_pages();
// 	std::string header = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\n";
// 	if (path.size() == 0)
// 	{
// 		std::string ll = "Content-Length: 118\n\r\n<html><head><title>501 Not Implemented</title></head><body><center><h1>501 Not Implemented</h1></center></body></html>";
// 		std::string l = header + ll;
// 		std::vector<char> res_vec(l.begin(), l.end());
// 		return res_vec;
// 	}
// 	std::vector<char> file_vec = getfileRaw(path + "/501.html");

// 	header += "Content-Length: " + std::to_string(file_vec.size()) + "\n\r\n";

// 	std::vector<char> return_vec(header.begin(), header.end());

// 	return_vec.insert(return_vec.end(), file_vec.begin(), file_vec.end());
// 	return return_vec;
// }