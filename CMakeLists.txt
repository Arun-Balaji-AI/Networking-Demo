# Build context file for entire aplication

cmake_minimum_required(VERSION 3.20)

option(WIN_CROSS "Windows cross compilation flag" OFF)

if (WIN_CROSS AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/mingw-w64.cmake CACHE FILEPATH "Toolchain File")
endif()

project(Network-demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(server)

add_executable(network-demo main.cpp)

if(MINGW)
    target_link_options(network-demo PRIVATE -static)
endif()

target_link_libraries(network-demo PRIVATE server)
