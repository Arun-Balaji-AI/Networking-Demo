#include <iostream>

#include "server.hpp"
#include <ws2tcpip.h>

Server::Server()
{
    std::cout << "[TRACE] Server Object created successfully." << std::endl;
}

bool Server::init()
{
    WORD wVersionRequired = MAKEWORD(2, 2);
    WSADATA wsaData;

    int result = WSAStartup(wVersionRequired, &wsaData);

    if (result != 0)
    {
        std::cout << "[ERROR] WSAStartup Failed. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] WSAStartup Success." << std::endl;

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET)
    {
        std::cout << "[ERROR] Error while creating socket. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] Socket created successfully." << std::endl;

    return true;
}

bool Server::bindSocket()
{
    sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &socketAddr.sin_addr.s_addr);
    socketAddr.sin_port = htons(55555);

    int result = bind(socket_, (sockaddr*)&socketAddr, sizeof(socketAddr));

    if (result == SOCKET_ERROR)
    {
        std::cout << "[ERROR] Error while binding socket. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] Socket is bound successfully." << std::endl;

    return true;
}

// A socket can be set to listening state only if it is
// TCP, if UDP is being used listen() is not needed.
bool Server::setListenState()
{
    int result = listen(socket_, 1);

    if (result == SOCKET_ERROR)
    {
        std::cout << "[ERROR] Error while setting the socket to listening state. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] Socket state set to listen successfully." << std::endl;
    return true;
}

bool Server::acceptConnections()
{
    listeningSocket_ = accept(socket_, nullptr, nullptr);

    if (listeningSocket_ == INVALID_SOCKET)
    {
        std::cout << "[ERROR] Error while accept() is initiated. " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "[TRACE] accept() is successfull." << std::endl;

    system("pause");
    return true;
}

void Server::close()
{
    WSACleanup();
    std::cout << "[TRACE] WSACleanup Success." << std::endl;
    closesocket(socket_);
    std::cout << "[TRACE] closesocket Sucess." << std::endl;
}
