# include "./NetWorking/Socket.hpp"




int     main(int argc, char **argv)
{

    Socket *s = new Socket();
    s->listen_on();


    delete s;

    return 0;
}
