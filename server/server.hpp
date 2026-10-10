#pragma once

#include<winsock2.h>
#include <atomic>

class Server
{
public:
    explicit Server(bool verbose);

    // Send data
    bool sendData(const char* payload, int payloadLen);

    // Receive data
    int receiveData(char* data, int dataLen);

    // sendLoop function
    void sendLoop();

    // receiveLoop function
    void receiveLoop();

    // shutdown the socket
    void shutdownSocket();

    // Cleanup function
    void close();

    bool isRunnable{false};
private:
    SOCKET socket_;
    SOCKET listeningSocket_;
    bool verbose_ {false};
    std::atomic<bool> running_{true};

    // Initialzer function
    bool init();

    // Binds the socket to server data
    bool bindSocket();

    // Sets the socket into listening state
    bool setListenState();

    // Accepts incoming connections
    bool acceptConnections();
}; // class Server
