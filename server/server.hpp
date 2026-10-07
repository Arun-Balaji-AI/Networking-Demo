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

    // Send data
    bool sendData(const char* payload, int payloadLen);

    // Receive data
    int receiveData(char* data, int dataLen);

    // Cleanup function
    void close();
private:
    SOCKET socket_;
    SOCKET listeningSocket_;
}; // class Server
