#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;
using ip::tcp;

class HttpServerSession : public std::enable_shared_from_this<HttpServerSession> {
public:
    HttpServerSession(boost::asio::io_service& io_service)
        : socket_(io_service) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(socket_, request_, "\r\n\r\n",
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                self->handle_read(error, bytes_transferred);
            });
    }

private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
        if (!error) {
            std::istream request_stream(&request_);
            std::string http_request;
            std::getline(request_stream, http_request);
            std::cout << "Received HTTP request: " << http_request << std::endl;

            std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
            async_write(socket_, boost::asio::buffer(response),
                [self = shared_from_this()](const boost::system::error_code& error, size_t /*bytes_transferred*/) {
                    self->handle_write(error);
                });
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }
    }

    tcp::socket socket_;
    boost::asio::streambuf request_;
};

class HttpServer {
public:
    HttpServer(boost::asio::io_service& io_service)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8080)),
          io_service_(io_service) {
        start_accept();
    }

private:
    void start_accept() {
        auto new_session = std::make_shared<HttpServerSession>(io_service_);

        acceptor_.async_accept(new_session->socket(),
            [this, new_session](const boost::system::error_code& error) {
                handle_accept(new_session, error);
            });
    }

    void handle_accept(std::shared_ptr<HttpServerSession> session, const boost::system::error_code& error) {
        if (!error) {
            session->start();
        }

        start_accept();
    }

    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

int main() {
    boost::asio::io_service io_service;
    HttpServer server(io_service);

    io_service.run();

    return 0;
}
