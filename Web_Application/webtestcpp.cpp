#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    // 这里处理接收到的消息，可以是数据采集程序发送过来的数据
    std::string data = msg->get_payload();
    std::cout << "Received data: " << data << std::endl;

    // 这里可以将接收到的数据发送给其他客户端
    s->send(hdl, data, websocketpp::frame::opcode::TEXT);
}

int main() {
    server srv;
    srv.set_message_handler(&on_message);

    // 这里设置服务器的端口号
    int port = 9002;

    try {
        srv.init_asio();
        srv.listen(port);
        srv.start_accept();

        std::cout << "Server listening on port " << port << std::endl;

        srv.run();
    } catch (websocketpp::exception const& e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown error" << std::endl;
    }

    return 0;
}
