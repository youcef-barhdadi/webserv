#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

class Request
{

	public:

		Request();

		Request( std::string &  src );
		~Request();

		Request &		operator=( Request const & rhs );


		void Parser();

	private:
		std::string  _accetpt_languge;
		std::string _accetpt_charset;
		std::string  _allow;
		std::string  verbe;
		std::string target



};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */