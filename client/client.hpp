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

    // Send the payload to server
    bool sendData(const char* payload, int payloadLen);

    // Receive the data from server
    int receiveData(char* data, int dataLen);

    // Cleanup
    void close();
private:
    SOCKET socket_;
}; //class Client
