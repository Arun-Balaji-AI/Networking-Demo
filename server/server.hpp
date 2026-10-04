#pragma once

#include<winsock2.h>

class Server
{
public:
    explicit Server();

    // Initialzer function
    bool init();

    // Cleanup function
    void close();
private:
}; // class Server
