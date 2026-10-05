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

    system("pause");
    return true;
}

void Client::close()
{
    WSACleanup();
    std::cout << "[TRACE] WSACleanup success." << std::endl;
    closesocket(socket_);
    std::cout << "[TRACE] closesocket success." << std::endl;
}
