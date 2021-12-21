
#include <sys/select.h>
#include <cassert>




class FileDescriptorManager
{

    public:
        static fd_set fd_set;


        static void Clean()
        {
            FD_ZERO(&fd_set);
        }

        static void Add(int fd)
        {
                assert(fd > 0);
                FD_SET(fd, &fd_set);
        }


        static bool Has(int fd)
        {
            assert( fd > 0);
            return FD_ISSET(fd, &fd_set);
        }


        static  void remove(int  fd)
        {
            assert(fd > 0);
            FD_CLR(fd, &fd_set);
        }

};