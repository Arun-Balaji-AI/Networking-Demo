// Main driver file

#include <iostream>
#include <string_view>

#include <server/server.hpp>
#include <client/client.hpp>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    std::cout << "[TRACE] Staring Initialization..." << std::endl;

    bool clientMode = false;
    if (argc > 0)
    {    
        std::cout << "[TRACE] Command-line arguments: ";
        for (int i = 1; i < argc; i++)
        {
            if (std::string_view(argv[i]) == "Client")
            {
                clientMode = true;
            }
            std::cout << argv[i] << std::endl;
        }
    }

    // Runs client if the binary/executable is started with Client
    if (clientMode)
    {
        std::cout << "[TRACE] Starting Client..." << std::endl;
        Client clientObj;

        if (!clientObj.init())
        {
            clientObj.close();
            return 1;
        }

        if (!clientObj.connectToServer())
        {
            clientObj.close();
            return 1;
        }

        std::string line;
        char buffer[BUFFER_SIZE];

        while(std::getline(std::cin, line) && line != "quit")
        {
            line += "\n";
            bool success = clientObj.sendData(line.c_str(), line.length());

            if (!success)
            {
                break;
            }

        }

        clientObj.close();

        return 0;
    }

    Server serverObj;

    if (!serverObj.init())
    {
        serverObj.close();
        return 1;
    }

    if (!serverObj.bindSocket())
    {
        serverObj.close();
        return 1;
    }

    if (!serverObj.setListenState())
    {
        return 1;
    }

    if (!serverObj.acceptConnections())
    {
        return 1;
    }

    char buffer[BUFFER_SIZE];

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int dataSize = serverObj.receiveData(buffer, BUFFER_SIZE);

        if (dataSize == 0)
        {
            break;
        }

        buffer[dataSize - 1] = '\0';

        std::cout << "Data: " << buffer << std::endl;
    }

    serverObj.close();

    return 0;
}
