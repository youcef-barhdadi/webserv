#include <sys/select.h>
#include "FileDescriptorManager.hpp"


fd_set FileDescriptorManager::set;

void  FileDescriptorManager::CLEAN()
{
        FD_ZERO(&FileDescriptorManager::set);
}

void FileDescriptorManager::ADD(int fd)
{
    FD_SET(fd, &FileDescriptorManager::set);
}

void FileDescriptorManager::REMOVE(int fd)
{
    FD_CLR(fd, &FileDescriptorManager::set);
}
