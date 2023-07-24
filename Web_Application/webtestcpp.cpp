#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp> // 新版 Boost 中的 bind 替换
#include <boost/thread/thread.hpp> // 如果用到线程

using namespace boost::asio;
using ip::tcp;

class HttpServerSession {
public:
    HttpServerSession(boost::asio::io_service& io_service)
        : socket_(io_service) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        boost::asio::async_read_until(socket_, request_, "\r\n\r\n",
            boost::bind(&HttpServerSession::handle_read, this, boost::asio::placeholders::error));
    }

private:
    void handle_read(const boost::system::error_code& error) {
        if (!error) {
            std::istream request_stream(&request_);
            std::string http_request;
            std::getline(request_stream, http_request);
            std::cout << "Received HTTP request: " << http_request << std::endl;

            std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
            boost::asio::async_write(socket_, boost::asio::buffer(response),
                boost::bind(&HttpServerSession::handle_write, this, boost::asio::placeholders::error));
        }
        else {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }

        delete this;
    }

    tcp::socket socket_;
    boost::asio::streambuf request_;
};

class HttpServer {
public:
    HttpServer(boost::asio::io_service& io_service)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8080)) {
        start_accept();
    }

private:
    void start_accept() {
        boost::shared_ptr<HttpServerSession> new_session =
            boost::make_shared<HttpServerSession>(boost::ref(io_service_)); // 使用新版 Boost 的 make_shared

        acceptor_.async_accept(new_session->socket(),
            boost::bind(&HttpServer::handle_accept, this, new_session, boost::asio::placeholders::error));
    }

    void handle_accept(boost::shared_ptr<HttpServerSession> session, const boost::system::error_code& error) {
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

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service)); // 如果用到线程

    io_service.run(); // 或者直接在主线程中运行，如果不用到线程的话

    return 0;
}
