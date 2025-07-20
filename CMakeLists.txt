cmake_minimum_required(VERSION 3.10)
project(FederatedLearning)

set(CMAKE_CXX_STANDARD 17)

add_definitions(
    -DASIO_STANDALONE
    -D_WEBSOCKETPP_CPP11_STL_
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/websocketpp
    ${CMAKE_SOURCE_DIR}/external/asio/asio/include
)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)
