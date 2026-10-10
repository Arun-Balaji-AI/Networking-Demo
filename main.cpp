// Main driver file

#include <iostream>
#include <string_view>
#include <thread>

#include <server/server.hpp>
#include <client/client.hpp>
#include <traces/traces.hpp>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    bool clientMode = false;
    bool verbose = false;

    if (argc > 1)
    {    
        for (int i = 1; i < argc; i++)
        {
            if (std::string_view(argv[i]) == "Client")
            {
                clientMode = true;
            }

            if (std::string_view(argv[i]) == "--verbose" || std::string_view(argv[i]) == "-v")
            {
                verbose = true;
            }

        }

    }

    // Runs client if the binary/executable is started with Client
    if (clientMode)
    {
        Client clientObj(verbose);

        if (!clientObj.isRunnable)
        {
            return 1;
        }

        std::thread clientReceiver(&Client::receiveLoop, &clientObj);
        clientObj.sendLoop();

        clientObj.shutdownSocket();
        clientReceiver.join();

        clientObj.close();

        return 0;
    }

    Server serverObj(verbose);

    if (!serverObj.isRunnable)
    {
        return 1;
    }

    std::thread serverReceiver(&Server::receiveLoop, &serverObj);
    serverObj.sendLoop();

    serverObj.shutdownSocket();
    serverReceiver.join();

    serverObj.close();

    return 0;
}
