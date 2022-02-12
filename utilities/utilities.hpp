/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarhdad <ybarhdad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 08:51:09 by ybarhdad          #+#    #+#             */
/*   Updated: 2022/02/03 15:40:22 by ybarhdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

std::vector<char> getfileRaw(std::string file);

int getSizeOfile(std::string file);

long FdGetFileSize(int fd);

int getSizeOfile(std::string file);

std::string	getExtension(std::string file);