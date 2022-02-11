#include "Request.hpp"
#include <vector>
#include <sstream>


using namespace std;

// funcation that i will use 

vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}




/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
{
}



Request::Request(Request const &req)
{
	this->is_finshed = req.is_finshed;
	this->keepAlive = req.keepAlive;
	this->_path = req.getPath();
}

Request::Request(  std::string & src  , int connection_fd)
{
	// set socket fd 
	this->connection_fd = connection_fd;
	this->is_finshed = true;
 	std::vector<std::string> lines =  split(src, '\n');
	int i = 0;
	while (i < lines.size())
	{
		// cout <<  "==>>>>>>>" << lines[i] << endl;

		// first argument is HTTP Verb  and requested resource
		if (i == 0)
		{
			std::vector<std::string> words = split(lines[i], ' ');
			this->setPath(words[1]);
		}
		else {

			if ( lines[i].find(' ')  !=std::string::npos)
			{
				std::string  first = lines[i].substr(0, lines[i].find(' '));
				first.pop_back();
				std::string  tow = lines[i].substr(lines[i].find(' '));
				this->header.insert(pair<std::string, std::string>( first, tow));
				  std::map<std::string, std::string>::iterator it;
				it = this->header.find(first);
			}
		}
		i++;
	}
	this->is_finshed = true;
}


int Request::getConnectinFD()
{
	return this->connection_fd;

}



bool	Request::getKeepALive()
{
	return this->keepAlive;
}






/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Request &				Request::operator=( Request const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Request const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/



std::string  Request::getPath() const
{
	return this->_path;
}


void	 Request::setPath(std::string path)
{
	this->_path = path;
}


/* ************************************************************************** */