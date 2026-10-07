#include <iostream>

#include "client.hpp"
#include <ws2tcpip.h>


Client::Client()
{
    std::cout << "[TRACE] Client Object created successfully." << std::endl;
}

bool Client::init()
{
    WORD wVersionRequired = MAKEWORD(2, 2);
    WSADATA wsaData;

    int result = WSAStartup(wVersionRequired, &wsaData);

    if (result != 0)
    {
        std::cout << "[ERROR] WSAStartup failed. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] WSAStartup success." << std::endl;

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_ == INVALID_SOCKET)
    {
        std::cout << "[ERROR] Error while creating socket. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] Socket created successfully." << std::endl;
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
        std::cout << "[ERROR] Connect failed. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] Connection to server successfull.\n Now client can send or receive data from server," << std::endl;

    return true;
}

bool Client::sendData(const char* payload, int payloadLen)
{
    int success = send(socket_, payload, payloadLen, 0);

    if (success == SOCKET_ERROR)
    {
        std::cout << "[ERROR] Error while sending the data. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] Data sent successfully." << std::endl;

    return true;
}

int Client::receiveData(char* data, int dataLen)
{
    int packets;
    int totalBytes = 0;

    while ((packets = recv(socket_, data + totalBytes, dataLen - totalBytes - 1, 0)))
    {
        data[totalBytes] = '\0';
        if (data[totalBytes] - 1 == '\n')
        {
            break;
        }
    }

    if (packets == SOCKET_ERROR)
    {
        std::cout << "[ERROR] Error while receiving the data. " << WSAGetLastError() << std::endl;
        return 0;
    }

    std::cout << "[TRACE] Data received successfully" << std::endl;

    return totalBytes;
}

void Client::close()
{
    WSACleanup();
    std::cout << "[TRACE] WSACleanup success." << std::endl;
    closesocket(socket_);
    std::cout << "[TRACE] closesocket success." << std::endl;
}
