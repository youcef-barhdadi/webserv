<<<<<<< HEAD

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

=======
#pragma  once 

#include <sys/select.h>

class FileDescriptorManager
{
    public:
        static fd_set set;

        static void  CLEAN();
        static void ADD(int fd);
        static void REMOVE(int fd);
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
};