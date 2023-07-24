#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
class HttpServer : public boost::enable_shared_from_this<HttpServer> {
public:
    HttpServer(boost::asio::io_service& io_service)
        : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080)),
          socket_(io_service) {
        start_accept();
    }

private:
    void start_accept() {
        acceptor_.async_accept(socket_,
            boost::bind(&HttpServer::handle_accept, this,
                        boost::asio::placeholders::error));
    }

    void handle_accept(const boost::system::error_code& error) {
        if (!error) {
            boost::make_shared<HttpServerSession>(boost::ref(socket_))->start();
        }

        start_accept();
    }

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};

class HttpServerSession : public boost::enable_shared_from_this<HttpServerSession> {
public:
    HttpServerSession(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start() {
        boost::asio::async_read_until(socket_, request_, "\r\n\r\n",
            boost::bind(&HttpServerSession::handle_read, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::istream request_stream(&request_);
            std::string request_line;
            std::getline(request_stream, request_line);

            // 处理请求，并准备响应
            std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
            boost::asio::async_write(socket_, boost::asio::buffer(response),
                boost::bind(&HttpServerSession::handle_write, shared_from_this(),
                            boost::asio::placeholders::error));
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            // 关闭连接
            boost::system::error_code ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        }
    }

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf request_;
};
int main() {
    try {
        boost::asio::io_service io_service;
        HttpServer server(io_service);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
