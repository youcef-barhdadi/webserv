/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 08:51:09 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/03/01 19:08:58 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
# include <vector>
# include <iostream>
# include <fstream>
# include <sys/stat.h>
# include <vector>
# include <sstream>
# include <string>
# include <ctime>
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <ctime>
# include <map>


typedef struct{
    bool                 isDir;
    std::string         fileName;
    std::string         date;
    std::string         size;
}   FileInfo;



std::vector<char> getfileRaw(std::string file);
int getSizeOfile(std::string file);
long FdGetFileSize(int fd);
int getSizeOfile(std::string file);
std::string	getExtension(std::string file);
std::vector<std::string> split(const std::string &s, char delim);
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

// random string generator
std::string RandString(int len);
// Hexadecimal number to Decimal
size_t     HexToDec(std::string nb);
bool 				isDirectory(std::string path);
std::vector<FileInfo>	getListOfFiles(std::string path);
std::string			get_time(time_t unix_timestamp);
double			get_time2(time_t begin);
std::string		generate_query_string(std::map<std::string, std::string>  query);