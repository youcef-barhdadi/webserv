#pragma  once 

#include <sys/select.h>

class FileDescriptorManager
{
    public:
        static fd_set set;

        static void  CLEAN();
        static void ADD(int fd);
        static void REMOVE(int fd);
};