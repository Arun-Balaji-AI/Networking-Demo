#pragma once

#include <winsock2.h>

class Client
{
public:
    explicit Client();

    // Initialization
    bool init();

    // Creates a connection to the server
    bool connectToServer();

    // Cleanup
    void close();
private:
    SOCKET socket_;
}; //class Client
