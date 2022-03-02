#include <sys/select.h>
#include "FileDescriptorManager.hpp"


fd_set FileDescriptorManager::set_read;
fd_set FileDescriptorManager::set_write;
int FileDescriptorManager::WRITE = 1;
int FileDescriptorManager::READ = 0;


void  FileDescriptorManager::CLEAN()
{
        FD_ZERO(&FileDescriptorManager::set_read);
        FD_ZERO(&FileDescriptorManager::set_write);
}

void FileDescriptorManager::ADD(int flag,int fd)
{   
    if (flag == FileDescriptorManager::WRITE)
        FD_SET(fd, &FileDescriptorManager::set_write);
    else
        FD_SET(fd, &FileDescriptorManager::set_read);

}

void FileDescriptorManager::REMOVE(int flag,int fd)
{
    // std::cout <<flag <<  "==========="<< FileDescriptorManager::WRITE<<  "\n";
    if (flag == FileDescriptorManager::WRITE)
        FD_SET(fd, &FileDescriptorManager::set_write);
    else
        FD_SET(fd, &FileDescriptorManager::set_read);
}
