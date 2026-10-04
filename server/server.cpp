#include <iostream>

#include "server.hpp"

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

    return true;
}

void Server::close()
{
    WSACleanup();
}
