#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

using boost::asio::ip::tcp;

class HttpServer {
public:
  HttpServer(boost::asio::io_context& io_context, int port)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {}

  void Start() { DoAccept(); }

private:
  void DoAccept() {
    auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());
    acceptor_.async_accept(*socket, boost::bind(&HttpServer::HandleAccept, this, socket, boost::asio::placeholders::error));
  }

  void HandleAccept(std::shared_ptr<tcp::socket> socket, const boost::system::error_code& error) {
    if (!error) {
      auto session = std::make_shared<HttpSession>(std::move(socket), router_);
      session->Start();
    }
    DoAccept();
  }

private:
  tcp::acceptor acceptor_;
  std::unordered_map<std::string, RequestHandler> router_;
};

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
  HttpSession(tcp::socket socket, std::unordered_map<std::string, RequestHandler>& router)
      : socket_(std::move(socket)), router_(router) {}

  void Start() { DoRead(); }

private:
  void DoRead() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(buffer_), [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        request_.Append(buffer_.data(), bytes_transferred);
        if (request_.IsComplete()) {
          Response response;
          auto handler = router_[request_.GetPath()];
          if (handler) {
            handler(request_, response);
          } else {
            response.SetStatusCode(404);
            response.SetStatusMessage("Not Found");
            response.SetBody("404 Not Found");
          }
          DoWrite(response);
        } else {
          DoRead();
        }
      }
    });
  }

  void DoWrite(const Response& response) {
    auto self(shared_from_this());
    std::vector<boost::asio::const_buffer> buffers = response.ToBuffers();
    boost::asio::async_write(socket_, buffers, [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
      }
    });
  }

private:
  tcp::socket socket_;
  std::unordered_map<std::string, RequestHandler>& router_;
  std::array<char, 8192> buffer_;
  Request request_;
};

int main() {
  boost::asio::io_context io_context;
  HttpServer server(io_context, 8080);
  server.Start();
  io_context.run();
  return 0;
}
