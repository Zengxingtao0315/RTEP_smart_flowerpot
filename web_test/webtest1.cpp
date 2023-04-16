#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>



void start_server_and_send_data() {
    // 创建Socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        std::cerr << "Failed to create socket!" << std::endl;
        return;
    }

    // 绑定Socket到本地地址和端口
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to bind socket!" << std::endl;
        close(sock);
        return;
    }

    // 监听连接请求
    if (listen(sock, 1) == -1) {
        std::cerr << "Failed to listen socket!" << std::endl;
        close(sock);
        return;
    }

    // 等待连接
    std::cout << "Waiting for connection..." << std::endl;
    struct sockaddr_in clientAddr = { 0 };
    socklen_t addrLen = sizeof(clientAddr);
    int clientSock = accept(sock, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSock == -1) {
        std::cerr << "Failed to accept connection!" << std::endl;
        close(sock);
        return;
    }

    std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // 实时输出参数到网络
    int i = 0;
    while (true) {
        std::string data = std::to_string(i);
        if (send(clientSock, data.c_str(), data.length(), 0) == -1) {
            std::cerr << "Failed to send data!" << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
        i++;
    }

    // 关闭连接和Socket
    close(clientSock);
    close(sock);
}
/*
void process_received_data(const std::string& data) {
    // 解析JSON数据
    rapidjson::Document doc;
    doc.Parse(data.c_str());

    // 提取参数值
    int time = doc["time"].GetInt();
    double temperature = doc["temperature"].GetDouble();
    double humidity = doc["humidity"].GetDouble();
    double light = doc["light"].GetDouble();

    // 在控制台输出参数值
    std::cout << "Received data:" << std::endl;
    std::cout << "time: " << time << std::endl;
    std::cout << "temperature: " << temperature << std::endl;
    std::cout << "humidity: " << humidity << std::endl;
    std::cout << "light: " << light << std::endl;
}
*/
int main() {
    // 创建一个线程用于发送数据到服务器
    std::thread data_sender_thread(send_data_to_server);

    // 启动服务器并接收数据
    //start_server_and_receive_data(process_received_data);

    // 等待数据发送线程结束
    data_sender_thread.join();

    return 0;
}
