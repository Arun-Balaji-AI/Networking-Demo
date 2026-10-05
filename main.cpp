// Main driver file

#include <iostream>
#include <string_view>

#include <server/server.hpp>
#include <client/client.hpp>

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

    serverObj.close();

    return 0;
}
