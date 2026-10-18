#pragma once

#include <iostream>
#include <string>
#include <mutex>

namespace Utils
{
constexpr int BUFFER_SIZE = 1024;
constexpr const char* recvLine = "< ";
constexpr const char* sendLine = "> ";

#pragma pack(push, 1)

struct payloadData_t
{
    char buffer[BUFFER_SIZE];
    int fromId = -1;
    int sendId = -1;

    void clear()
    {
        buffer[0] = '\0';
        fromId = -1;
        sendId = -1;
    }
};

#pragma pack(pop)

inline std::mutex& commonLock()
{
    static std::mutex m;
    return m;
}

inline void printTraces(const std::string& message, bool isError)
{
    std::lock_guard<std::mutex> lock(commonLock());

    if (isError)
    {
        std::cout << "[ERROR] " << message << std::endl;
        return;
    }

    std::cout << "[TRACE] " << message << std::endl;
}

} // namespace Trace
