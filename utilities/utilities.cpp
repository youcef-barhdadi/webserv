/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztaouil <ztaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 08:51:06 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/14 20:57:32 by ztaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"




std::vector<char>	getRange(std::string file, size_t start, size_t end)
{
	if (start == 0 && end ==0)
		return (getfileRaw(file));
	std::ifstream iffile(file,  std::ios::in|std::ios::binary|std::ios::ate);
	std::streampos size;
	char* memblock;
	int sizee;
	std::vector<char>   empty;
	if (iffile.is_open())
	{
		size = end - start;
		sizee = size;
		memblock = new char [sizee];
		iffile.seekg (start, std::ios::beg);
		iffile.read (memblock, size);
		iffile.close();
		std::vector<char> ret(memblock, memblock+sizee);
		return ret;
	}
    return empty;	
}




std::vector<char> getfileRaw(std::string file)
{
	std::ifstream iffile(file,  std::ios::in|std::ios::binary|std::ios::ate);
	std::streampos size;
	char* memblock;
	int sizee;
	// stup
	std::vector<char>   empty;

	if (iffile.is_open())
	{
		size = iffile.tellg();
		sizee = size;
		memblock = new char [sizee];
		iffile.seekg (0, std::ios::beg);
		iffile.read (memblock, size);
		iffile.close();
		std::vector<char> ret(memblock, memblock+sizee);
		return ret;
	}
    return empty;
}

long FdGetFileSize(int fd)
{
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::string	getExtension(std::string file) 
{
	std::string str = file.substr(file.find('.')+1);

	return str;
} 


int getSizeOfile(std::string file)
{
   std::ifstream in_file(file, std::ios::binary);
   in_file.seekg(0, std::ios::end);
   int file_size = in_file.tellg();
	in_file.close();
	return file_size;
}


std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline(ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
 
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

std::string RandString(int len)
{
    srand(time(NULL) * getpid());
    const std::string alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string str;
    str.reserve(len);

    for(int i = 0; i < len; ++i){
        str += alphanum[rand() % (alphanum.size() - 1)];
    }
    return str;
}

size_t     HexToDec(std::string nb)
{
    size_t n;
    std::string tmp;
    std::stringstream ss;

    ss << nb;
    ss >> std::hex >> n;
    
    return n;
}