#include "client.hpp"
#include <ws2tcpip.h>
#include <thread>
#include <conio.h>

#include<traces/traces.hpp>

#define BUFFER_SIZE 1024

Client::Client()
{
    Trace::printTraces("Client Object created successfully.", false);

    Trace::printTraces("Starting init()...", false);
    isRunnable = init();

    if (isRunnable)
    {
        isRunnable = connectToServer();
    }

    if (!isRunnable)
    {
        close();
    }
}

bool Client::init()
{
    WORD wVersionRequired = MAKEWORD(2, 2);
    WSADATA wsaData;

    int result = WSAStartup(wVersionRequired, &wsaData);

    if (result != 0)
    {
        Trace::printTraces("WSAStartup failed. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    Trace::printTraces("WSAStartup success.", false);

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_ == INVALID_SOCKET)
    {
        Trace::printTraces("Error while creating socket. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    Trace::printTraces("Socket created successfully.", false);
    return true;
}

bool Client::connectToServer()
{
    sockaddr_in socketAddr;

    socketAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &socketAddr.sin_addr.s_addr);

    socketAddr.sin_port = htons(55555);

    int result = connect(socket_, (sockaddr*) &socketAddr, sizeof(socketAddr));

    if (result == SOCKET_ERROR)
    {
        Trace::printTraces("Connect failed. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    Trace::printTraces("Connection to server successfull. Now client can send or receive data from server...", false);

    return true;
}

bool Client::sendData(const char* payload, int payloadLen)
{
    int success = send(socket_, payload, payloadLen, 0);

    if (success == SOCKET_ERROR)
    {
        Trace::printTraces("Error while sending the data. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    Trace::printTraces("Data sent successfully.", false);

    return true;
}

int Client::receiveData(char* data, int dataLen)
{
    Trace::printTraces("Receiving data...", false);
    int packets;
    int totalBytes = 0;

    while ((packets = recv(socket_, data + totalBytes, dataLen - totalBytes - 1, 0)) > 0)
    {
        totalBytes += packets;
        data[totalBytes] = '\0';
        if (data[totalBytes - 1] == '\n')
        {
            break;
        }
    }

    if (packets == SOCKET_ERROR)
    {
        Trace::printTraces("Error while receiving the data. " + std::to_string(WSAGetLastError()), true);
        return 0;
    }

    Trace::printTraces("Data received successfully", false);

    return totalBytes;
}

void Client::sendLoop()
{
    std::string line;
    while (running_ && std::getline(std::cin, line))
    {
        if (line == "quit")
        {
            break;
        }

        line += "\n";

        int success = sendData(line.c_str(), line.length());

        if (success <= 0)
        {
            Trace::printTraces("Error while sending the data..." + std::to_string(WSAGetLastError()), true);
            break;
        }

        Trace::printTraces("Data sent successfully...", false);
    }

    running_ = false;
}

void Client::receiveLoop()
{
    char buffer[BUFFER_SIZE];

    while (running_)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int success = receiveData(buffer, BUFFER_SIZE);

        if (success <= 0)
        {
            Trace::printTraces("Error while receiving data..." + std::to_string(WSAGetLastError()), true);
            break;
        }

        buffer[success - 1] = '\0';

        std::cout << "> " << buffer << std::endl;
    }
}

void Client::shutdownSocket()
{
    ::shutdown(socket_, SD_BOTH);
}

void Client::close()
{
    WSACleanup();
    Trace::printTraces("WSACleanup success.", false);
    closesocket(socket_);
    Trace::printTraces("closesocket success.", false);
}
