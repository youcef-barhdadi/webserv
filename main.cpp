// # include "./NetWorking/Socket.hpp"
# include "./spinner/Spinner.hpp"
# include "./server/Server.hpp"


#include "FileDescriptorManager/FileDescriptorManager.hpp"
int     main(int argc, char **argv)
{


    
    Spinner s;
    Server *server = new Server();
    // Server *server = new Server();
// ≈÷= ;
        FD_ZERO(&FileDescriptorManager::set);

        
    server->addPort(1334);
    server->addPort(1332);
    server->addPort(1565);
    std::cout << server->_ports.size() << std::endl;
    s._servers.push_back(server);

    s.run();



    // serevr.a
    // Socket *s = new Socket();
    // s->listen_on();


    // delete s;

    return 0;
}
