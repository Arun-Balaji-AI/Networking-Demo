#pragma once

#include <winsock2.h>
#include <atomic>
#include <mutex>
#include <string>

class Client
{
public:
    explicit Client(bool verbose);

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
    bool verbose_ {false};

    std::string currMsg_;

    std::mutex ioMutex_;
    // Initialization
    bool init();

    // Creates a connection to the server
    bool connectToServer();

    void redrawMessage();

    void printMessage(const char* buffer);
}; //class Client
