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
        std::string response = generateHTMLResponse();
        async_write(socket_, boost::asio::buffer(response),
            [self = shared_from_this()](const boost::system::error_code& error, size_t /*bytes_transferred*/) {
                self->handle_write(error);
            });
    }

private:
    std::string generateHTMLResponse() {
        std::string html = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=0.5">
    <title>Intelligent Flower Pot</title>
    <style>
        /* CSS styles for your HTML content */
        /* ... */
    </style>
</head>
<body style="background-color: rgb(29, 29, 29);background-size: 100%">
    <!-- Your HTML content -->
    <!-- ... -->
</body>
</html>)";
        return html;
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }
    }

    tcp::socket socket_;
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
