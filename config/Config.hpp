#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>

class Config
{

	public:

		Config();
		Config( Config const & src );
		~Config();

		Config &		operator=( Config const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Config const & i );

#endif /* ********************************************************** CONFIG_H */