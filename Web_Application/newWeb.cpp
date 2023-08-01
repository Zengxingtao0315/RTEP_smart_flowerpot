#include <iostream>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <nlohmann/json.hpp> // JSON library, you need to install it first

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;
using websocket = beast::websocket;

// JSON library alias
using json = nlohmann::json;

void do_session(tcp::socket socket) {
    try {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        while (true) {
            // Simulated temperature and humidity data (replace with actual sensor data)
            double temperature = 25.5;
            double humidity = 50.0;

            // Create a JSON object to hold temperature and humidity
            json data;
            data["temperature"] = temperature;
            data["humidity"] = humidity;

            // Convert JSON object to string
            std::string message = data.dump();

            // Send the JSON string as WebSocket message to the client
            ws.write(asio::buffer(message));

            // Sleep for a short time before updating data again
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "WebSocket error: " << e.what() << std::endl;
    }
}

int main() {
    asio::io_context ioc;
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 8080));

    while (true) {
        tcp::socket socket(ioc);
        acceptor.accept(socket);
        std::thread(do_session, std::move(socket)).detach();
    }

    return 0;
}
