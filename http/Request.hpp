#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

#include <map>


// see this link to see why i peekup this names https://code.tutsplus.com/tutorials/http-headers-for-dummies--net-8039

class Request
{

	public:

		Request();

		Request( std::string &  src );
		~Request();

		Request &		operator=( Request const & rhs );


		void Parser();


		std::string  getPath() const;
		void	setPath(std::string path);




			std::map<std::string, std::string> header;

	private:
		std::string  _accetpt_languge;
		std::string _accetpt_charset;
		std::string  _allow;
		std::string  _method;
		std::string _target;
		std::string _path;




};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */