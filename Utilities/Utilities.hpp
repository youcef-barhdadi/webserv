/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztaouil <ztaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 08:51:09 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/24 16:51:58 by ztaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
# include <vector>
# include <sstream>
# include <string>
# include <ctime>
# include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


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
