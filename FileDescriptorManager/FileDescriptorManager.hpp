#pragma  once 

#include <sys/select.h>
#include <iostream>

class FileDescriptorManager
{
    public:
        static fd_set set_read;
        static fd_set set_write;
        static int WRITE ;
        static int READ;

        static void  CLEAN();
        static void ADD(int f,int fd);
        static void REMOVE(int f,int fd);
};