## Build Context file for server

add_library(server server.cpp)

target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)
    target_link_libraries(server PUBLIC ws2_32)
endif()
