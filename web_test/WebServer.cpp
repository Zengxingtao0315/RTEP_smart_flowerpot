#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

using websocketpp::server;

int main() {
    // Create a WebSocket server
    server<websocketpp::config::asio> ws_server;
    ws_server.init_asio();

    // Set up the server's callback functions
    ws_server.set_open_handler([&](websocketpp::connection_hdl hdl) {
        std::cout << "Client connected" << std::endl;
    });

    ws_server.set_close_handler([&](websocketpp::connection_hdl hdl) {
        std::cout << "Client disconnected" << std::endl;
    });

    // Set up a timer to periodically send updated data to clients
    boost::asio::deadline_timer timer(ws_server.get_io_service());
    timer.expires_from_now(boost::posix_time::seconds(1));
    timer.async_wait(boost::bind([&](const boost::system::error_code& ec) {
        if (!ec) {
            // Generate some data to send
            std::string data = "{\"time\": \"12:00\", \"temperature\": 23.5, \"humidity\": 60, \"light\": 100}";

            // Broadcast the data to all connected clients
            for (auto it = ws_server.get_connections().begin(); it != ws_server.get_connections().end(); ++it) {
                ws_server.send(*it, data, websocketpp::frame::opcode::text);
            }

            // Reschedule the timer
            timer.expires_from_now(boost::posix_time::seconds(1));
            timer.async_wait(boost::bind([&](const boost::system::error_code& ec) {
                // Handle timer expiration recursively
            }));
        }
    }, _1));

    // Start the server listening on port 12345
    ws_server.listen(12345);
    ws_server.start_accept();

    // Run the server
    ws_server.run();
}
