#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_STL_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

typedef websocketpp::client<websocketpp::config::asio_client> client;

void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

int main() {
    client c;

    try {
        c.set_access_channels(websocketpp::log::alevel::none);
        c.clear_access_channels(websocketpp::log::alevel::all);

        c.init_asio();
        c.set_message_handler(&on_message);

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection("ws://localhost:9002", ec);
        if (ec) {
            std::cout << "Connection initialization error: " << ec.message() << std::endl;
            return 1;
        }

        c.connect(con);

        // Run client in a thread so we can send after connecting
        std::thread t([&c]() { c.run(); });

        // Wait a bit to ensure connection established
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Send a message
        c.send(con->get_handle(), "Hello from C++ client", websocketpp::frame::opcode::text);

        // Wait a bit to receive echo
        std::this_thread::sleep_for(std::chrono::seconds(2));

        c.stop();
        t.join();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
