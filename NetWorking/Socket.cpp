
#include "Socket.hpp"
#include <fstream>



#include <stdlib.h>



Socket::Socket()
{

}


Socket::~Socket()
{
    close(this->_connection);
}



int Socket::getConnection()
{
    return this->_connection;
}



int Socket::create_Connection(){
            int server_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (server_fd < 0)
            {
                perror("error\n");
                exit(0);
            }
            this->_connection = server_fd;
         return server_fd;
}



void Socket::Create_server()
{
    struct sockaddr_in address;

    this->_port = 8080;

    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr= INADDR_ANY;
     address.sin_port =htons(this->_port);

   int ret =  bind(this->_connection, (struct sockaddr *)&address , sizeof(address));
   if (ret < 0)
   {
       perror("error");
       return ;
   }
}




void Socket::listen_on()
{
    this->create_Connection();
    this->Create_server();
    struct sockaddr_in address;
    socklen_t addrlen;
    size_t  readlen;
    char buffer[30000] = {0};
    int  ret  =   listen(this->_connection, 15);
    
    
    std::ifstream MyReadFile("index.html");

    std::string  str;

    std::string  hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
    std::string body = "";

    while (getline (MyReadFile, str)) {
  // Output the text from the file
    body +=  str;
}


std::string number = std::to_string(body.length());
hello +=  number + "\n\n" +  body;
// Close the file
MyReadFile.close();

    if (ret < 0)
    {
        perror("In listen");
        return ;
    }


    while (1)
    {
        int new_socket = accept(this->_connection, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (new_socket < 0)
        {
            perror("In accept");
            return ;
        }

        readlen = read(new_socket, buffer, 30000);
        buffer[readlen] =0;
        std::string copy = std::string(buffer);
        Request *request = new Request(copy);
        std::cout << buffer << std::endl;
        write(new_socket,  hello.c_str(),hello.length());
        close(new_socket);
    }
    close(this->_connection);


}