// Main driver file

#include <iostream>

#include <server/server.hpp>

int main(int argv, char* argc[])
{
    std::cout << "[TRACE] Staring Initialization..." << std::endl;

    Server serverObj;

    if (!serverObj.init())
    {
        return 0;
    }

    serverObj.close();

    return 1;
}
