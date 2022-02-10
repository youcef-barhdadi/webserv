#include <iostream>
#include <string>
#include <unistd.h>

int		main()
{
	char *str = "ztaouil\0mkaddani";

	std::string	str(str, 27);
	// const char *s = str.c_str();

	// std::cout << str.size() << std::endl;
	write(1, str, 20);
	return 0;
}