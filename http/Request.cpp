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

Request::Request(  std::string & src )
{
 	std::vector<std::string> lines =  split(src, '\n');
	int i = 0;
	while (i < lines.size())
	{
		cout <<  "==>>>>>>>" << lines[i] << endl;
		i++;
	}
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


/* ************************************************************************** */