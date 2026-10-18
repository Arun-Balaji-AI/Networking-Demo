#include <iostream>
#include <utils/utils.hpp>
#include<cstring>
#include <thread>
#include <conio.h>

#include "server.hpp"
#include <ws2tcpip.h>

#define BUFFER_SIZE 1024

Server::Server(bool verbose)
{
    verbose_ = verbose;

    if (verbose)
    {
        Utils::printTraces("Server object created successfully...", false);
        Utils::printTraces("---Starting init()---", false);
    }
    isRunnable = init();

    if (isRunnable)
    {
        isRunnable = bindSocket();
    }

    if (isRunnable)
    {
        isRunnable = setListenState();
    }

    if (isRunnable)
    {
        isRunnable = acceptConnections();
    }

    if (!isRunnable)
    {
        close();
    }
}

bool Server::init()
{
    WORD wVersionRequired = MAKEWORD(2, 2);
    WSADATA wsaData;

    int result = WSAStartup(wVersionRequired, &wsaData);

    if (result != 0)
    {
        if (verbose_)
            Utils::printTraces("WSAStartup Failed. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    if (verbose_)
        Utils::printTraces("WSAStartup Success.", false);

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET)
    {
        if (verbose_)
            Utils::printTraces("Error while creating socket. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    if (verbose_)
        Utils::printTraces("Socket created successfully.", false);

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
        if (verbose_)
            Utils::printTraces("Error while binding socket. " + std::to_string(WSAGetLastError()), false);
        return false;
    }

    if (verbose_)
        Utils::printTraces("Socket is bound successfully.", false);

    return true;
}

// A socket can be set to listening state only if it is
// TCP, if UDP is being used listen() is not needed.
bool Server::setListenState()
{
    int result = listen(socket_, 1);

    if (result == SOCKET_ERROR)
    {
        if (verbose_)
            Utils::printTraces("Error while setting the socket to listening state. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    if (verbose_)
        Utils::printTraces("Socket state set to listen successfully.", false);
    return true;
}

bool Server::acceptConnections()
{
    listeningSocket_ = accept(socket_, nullptr, nullptr);

    if (listeningSocket_ == INVALID_SOCKET)
    {
        if (verbose_)
            Utils::printTraces("Error while accept() is initiated. " + std::to_string(WSAGetLastError()), true);
        return false;
    }

    if (verbose_)
        Utils::printTraces("accept() is successfull.", false);

    return true;
}

bool Server::sendData(const char* payload, int payloadLen)
{
    int success = send(listeningSocket_, payload, payloadLen, 0);

    if (success == SOCKET_ERROR)
    {
        if (verbose_)
            Utils::printTraces("Payload not sent. " + std::to_string(WSAGetLastError()), true);
         return false;
    }

    if (verbose_)
        Utils::printTraces("Payload sent to client successfully.", false);
    return true;
}

int Server::receiveData(char* data, int dataLen)
{
    if (verbose_)
        Utils::printTraces("Receiving data...", false);
   int packets;
   int totalBytes = 0;

   while ((packets = recv(listeningSocket_, data + totalBytes, dataLen - totalBytes - 1, 0)) > 0)
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
       if (verbose_)
            Utils::printTraces("Error while receiving the data. " + std::to_string(WSAGetLastError()), true);
       return 0;
   }

   if (verbose_)
        Utils::printTraces("Data received successfully", false);

   return totalBytes;
}

void Server::sendLoop()
{
    {
        std::lock_guard<std::mutex> lock(ioMutex_);
        redrawMessage();
    }

    while(running_)
    {
        if (!_kbhit())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        int ch = _getch();

        if (ch == 0 || ch == 224)
        {
            continue;
        }

        if (ch == '\r')
        {
            std::string line = currMsg_;
            line += "\n";
            currMsg_.clear();
            std::cout << '\n';
            redrawMessage();

            if (line.empty() || line == "quit")
            {
                break;
            }


            std::strncpy(payloadData_.buffer, line.c_str(), sizeof(payloadData_.buffer) - 1);

            int success = sendData(line.c_str(), line.length());

            if (success <= 0)
            {
                break;
            }
        }
        else if (ch == 8)
        {
            std::lock_guard<std::mutex> lock(ioMutex_);
            if (!currMsg_.empty())
            {
                currMsg_.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        }
        else if (ch >= 32 && ch < 127)
        {
            std::lock_guard<std::mutex> lock(ioMutex_);
            currMsg_.push_back(static_cast<char>(ch));
            std::cout << static_cast<char>(ch) << std::flush;
        }
    }

    running_ = false;
}

void Server::receiveLoop()
{
    char buffer[BUFFER_SIZE];

    while(running_)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int success = receiveData(buffer, BUFFER_SIZE);

        if (success <= 0)
        {
            break;
        }

        buffer[success - 1] = '\0';
        
        printMessage(buffer);
    }
}

void Server::redrawMessage()
{
    std::cout << Utils::sendLine << currMsg_ << std::flush;
}

void Server::printMessage(const char* buffer)
{
    std::cout << '\r' << std::string(2 + currMsg_.size(), ' ') << '\r' << std::flush;

    std::cout << Utils::recvLine << buffer << std::endl;

    redrawMessage();
}

void Server::shutdownSocket()
{
    ::shutdown(listeningSocket_, SD_BOTH);
}

void Server::close()
{
    WSACleanup();
    if (verbose_)
        Utils::printTraces("WSACleanup Success.", false);
    closesocket(socket_);
    if (verbose_)
        Utils::printTraces("closesocket Sucess.", false);
}
