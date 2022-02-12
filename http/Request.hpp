#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

#include <map>
#include <vector>


// see this link to see why i peekup this names https://code.tutsplus.com/tutorials/http-headers-for-dummies--net-8039

class Request
{

	public:

		Request();

		Request( std::string &  src , int connection_fd);
		~Request();
Request (Request const &req);
		Request &		operator=( Request const & rhs );


		void Parser();


		std::string  getPath() const;
		void	setPath(std::string path);


		int getConnectinFD();

		std::map<std::string, std::string> header;

		bool	getKeepALive();
		bool		is_finshed;


		std::string		&getMethod();

	private:
		std::string  _accetpt_languge;
		std::string _accetpt_charset;
		std::string  _allow;
		std::string  _method;
		std::string _target;
		std::string _path;	
		int 		connection_fd;
		bool		keepAlive;




};

std::ostream &			operator<<( std::ostream & o, Request const & i );
std::vector<std::string> split (const std::string &s, char delim);

#endif /* ********************************************************* REQUEST_H */