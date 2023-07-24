#include <iostream>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <string>
#include <thread>
#include <chrono>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;
typedef WebSocketServer::message_ptr MessagePtr;

// 全局变量，模拟实时更新的数据
int realtimeData = 0;

void onRequest(evhttp_request* req, void* arg) {
    evhttp_send_error(req, HTTP_NOTIMPLEMENTED, "Not implemented");
}

int main() {
    event_base* base = event_base_new();
    if (!base) {
        std::cerr << "Failed to create event base." << std::endl;
        return -1;
    }

    evhttp* http = evhttp_new(base);
    if (!http) {
        std::cerr << "Failed to create evhttp." << std::endl;
        return -1;
    }

    evhttp_bind_socket(http, "0.0.0.0", 8080); // 监听地址和端口

    // 设置请求处理回调函数
    evhttp_set_gencb(http, onRequest, nullptr);

    // 创建WebSocket服务器
    WebSocketServer server;
    server.init_asio();

    // 设置WebSocket消息处理回调函数
    server.set_message_handler([&server](WebSocketServer* ws_server, websocketpp::connection_hdl hdl, MessagePtr msg) {
        std::string message = msg->get_payload();
        std::cout << "Received message: " << message << std::endl;
    });

    // 启动WebSocket服务器在8081端口
    server.listen(8081);

    // 启动WebSocket服务器的异步事件处理线程
    std::thread ws_thread([&server]() {
        server.start_accept();
        server.run();
    });

    // 模拟数据更新
    while (true) {
        realtimeData++; // 更新数据
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒更新一次数据
    }

    evhttp_free(http);
    event_base_free(base);
    return 0;
}
