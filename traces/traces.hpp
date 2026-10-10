#pragma once

#include <iostream>
#include <string>
#include <mutex>

namespace Trace
{
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
