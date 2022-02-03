/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 08:51:06 by ybarhdad          #+#    #+#             */
/*   Updated: 2021/12/21 08:51:07 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilities.hpp"





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