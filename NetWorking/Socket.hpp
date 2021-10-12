
#pragma once



#include <sys/socket.h>
#include "iostream"
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../http/Request.hpp"
#include "../http/Response.hpp"

     #include <sys/types.h>

       #include <netinet/in.h>
       #include <netinet/ip.h> 

class Socket{
    private:
            int _connection; // socket id
            int create_Connection();
            int _port;  // http protcol use 80 , but i will use 8080 for testing and not be in confilect if there any other server runong

    public:   
    Socket();
        int getConnection();
        void Create_server( );
        ~Socket();
void listen_on();
};
