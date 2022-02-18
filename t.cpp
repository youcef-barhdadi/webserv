#include <string>
#include <iostream>

int		main()
{
	std::string s1 = "/static/images";
	std::string s2 = "/sta2tic";

	std::string s3 = "/static/images/hamid.png";

	std::cout << s3.find(s1) << std::endl;
	std::cout << (s3.find(s2) == std::string::npos) << std::endl;

	return 0;
}