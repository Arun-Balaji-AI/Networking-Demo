#pragma once

#include<winsock2.h>

class Server
{
public:
    explicit Server();

    // Initialzer function
    bool init();

    // Binds the socket to server data
    bool bindSocket();

    // Sets the socket into listening state
    bool setListenState();

    // Accepts incoming connections
    bool acceptConnections();

    // Cleanup function
    void close();
private:
    SOCKET socket_;
    SOCKET listeningSocket_;
}; // class Server
