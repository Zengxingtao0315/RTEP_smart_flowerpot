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
    std::string HttpServerSession::generateHTMLResponse() {
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=0.5">
    <title>Intelligent Flower Pot</title>
    <style>
        #id01 {background-color: rgb(85, 85, 85);
        }

        .main_1 {
            border-radius: 15px;
            margin: 7px;
        }

        .font_1 {
            color: rgb(255,255,255);
            text-align: center;
        }
    </style>
</head>
<body style="background-color: rgb(29, 29, 29);background-size: 100% ">
    <!-- main body -->
    <div style="height:800px;width: 800px;margin: auto;display:flex;flex-wrap: wrap; flex-direction: column;">
        <!-- top -->
        <div style="background-color: green; height: 100px;  text-align: center; border-radius: 15px;margin:0 auto; margin:  7px 7px 14px 7px;">
            <h2 style="color: white; line-height: 50px;">Real-time Reflections of Your Lovely Plant</h2>
        </div>
        <!-- middle -->
        <!-- middle-top -->
        <div class='main_1 ' style="height: fit-content; padding: 10px;background-color: rgb(59, 59, 59); text-align:center;">
            <h1 class='font_1' style='font-size: 50px;line-height:40px; '>
                {{time}}
            </h1>
            <button onclick="location.reload()" style = "background-color: rgb(59, 59, 59); cursor: pointer;  color: white;border: none; font-size: 20px; margin-top:30px;">Refresh</button>
        </div>
        <!-- middle-bottom -->
        <div style = "display:flex;justify-content: space-between; flex-direction: row;">
            <div class='main_1 block Temperature' style="height: 300px;float:right;padding: 15px;background-color: rgb(100, 200, 100);text-align: center; width :33%;">
                <h2 class='font_1' style='font-size: 30px;line-height:100px; '>
                    Temperature
                </h2>
                <h2 class='font_1' style='font-size: 30px;line-height:30px; '>
                    {{temperature}}<br>&#176;C
                </h2>
            </div>
            <div class='main_1 block Humidity' style="height: 300px;float:right;padding: 15px;background-color: rgb(0, 0, 255); text-align:center;width :33%;">
                <h2 class='font_1' style='font-size: 30px;line-height:40px;'>
                    Humidity
                </h2>
                <h2 class='font_1' style="font-size: 30px;line-height:30px;margin: 7px; padding: 15px;">
                    {{humidity}}<br>%
                </h2>
            </div>

            <div class='main_1 block light' style="height: 300px;float:right;padding: 15px;background-color: rgb(255, 165, 0);text-align:center;width :33%; ">
                <a class='font_1' style="font-size: 30px;line-height:30px; padding: 10px;margin-top:10px ">
                    {{lightFlag}}
                </a>
            </div>
        </div>
        <!-- bottom -->
        <div style='height: 50;margin: 0 5px 5px 5px;'>
            <p style='color:rgb(151, 255, 255);text-align: center;'>Copyright&copy;G27_Real Time Embedded Programming(2022-23)
                <br>This website is only for academic demo purpose.<br></p>
        </div>
    </div>
</body>
</html>
)";
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
