#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main() {
    try {
        io_context io;

        // 创建TCP监听器
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8080));

        while (true) {
            tcp::socket socket(io);
            acceptor.accept(socket);

            std::string message = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
            boost::system::error_code ignored_error;
            write(socket, buffer(message), ignored_error);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
