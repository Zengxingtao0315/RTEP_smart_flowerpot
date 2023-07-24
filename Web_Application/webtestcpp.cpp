#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

typedef websocketpp::server<websocketpp::config::asio> server;

int main() {
    // 创建WebSocket服务器
    server ws_server;

    // 设置监听端口
    int port = 9002;
    ws_server.listen(port);

    // 启动服务器线程
    ws_server.start_accept();

    std::cout << "WebSocket server started on port " << port << std::endl;

    // 模拟传感器数据，并实时发送到HTML网页
    double sensor_data = 0.0;

    while (true) {
        // 模拟获取传感器数据，你需要替换此处的代码以读取真实的传感器数据
        // 假设传感器数据在sensor_data中更新
        sensor_data += 0.1;

        // 将数据转换成字符串
        std::string data_str = std::to_string(sensor_data);

        // 遍历所有连接的客户端，发送数据
        for (auto it : ws_server.get_connections()) {
            try {
                ws_server.send(it, data_str, websocketpp::frame::opcode::text);
            } catch (const websocketpp::exception &e) {
                std::cout << "Error sending data to client: " << e.what() << std::endl;
            }
        }

        // 等待一段时间，模拟传感器数据的更新频率
        std::this_thread::sleep_for(1s);
    }

    return 0;
}
