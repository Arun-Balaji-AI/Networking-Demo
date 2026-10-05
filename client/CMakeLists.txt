## Build context file for client

add_library(client client.cpp)

target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)
    target_link_libraries(client PUBLIC ws2_32)
endif()
