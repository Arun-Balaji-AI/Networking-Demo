#pragma once

#include <winsock2.h>
#include <atomic>

class Client
{
public:
    explicit Client();

    // Send the payload to server
    bool sendData(const char* payload, int payloadLen);

    // Receive the data from server
    int receiveData(char* data, int dataLen);

    // sendLoop function
    void sendLoop();

    // receiveLoop function
    void receiveLoop();

    void shutdownSocket();

    // Cleanup
    void close();

    bool isRunnable{false};
private:
    SOCKET socket_;
    std::atomic<bool> running_ {true};

    // Initialization
    bool init();

    // Creates a connection to the server
    bool connectToServer();
}; //class Client
