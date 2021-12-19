
#include <sys/select.h>
#include <cassert>




class FileDescriptorManager
{

    public:
        static fd_set fd_set;

        static void add(int fd)
        {
                assert(fd > 0);
                FD_SET(fd, &fd_set);
        } 








};