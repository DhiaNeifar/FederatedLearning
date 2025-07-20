#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_STL_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>

// For std::bind
#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;

// To store active connections (optional for now)
std::set<connection_hdl, std::owner_less<connection_hdl>> connections;

void on_open(server* s, connection_hdl hdl) {
    std::cout << "Client connected\n";
    connections.insert(hdl);
}

void on_close(server* s, connection_hdl hdl) {
    std::cout << "Client disconnected\n";
    connections.erase(hdl);
}

void on_message(server* s, connection_hdl hdl, server::message_ptr msg) {
    std::string payload = msg->get_payload();
    std::cout << "Received: " << payload << std::endl;

    // Echo back the message to the sender
    s->send(hdl, payload, msg->get_opcode());
}

int main() {
    server echo_server;

    try {
        // Set event handlers
        echo_server.set_open_handler(std::bind(&on_open, &echo_server, std::placeholders::_1));
        echo_server.set_close_handler(std::bind(&on_close, &echo_server, std::placeholders::_1));
        echo_server.set_message_handler(std::bind(&on_message, &echo_server,
                                                  std::placeholders::_1, std::placeholders::_2));

        // Initialize Asio
        echo_server.init_asio();

        // Listen on port 9002
        echo_server.listen(9002);

        // Accept incoming connections
        echo_server.start_accept();

        std::cout << "Server is running on ws://localhost:9002\n";

        // Start the event loop
        echo_server.run();
    } catch (websocketpp::exception const& e) {
        std::cerr << "WebSocket++ exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Other exception\n";
    }

    return 0;
}
