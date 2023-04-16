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

using namespace std::chrono_literals;

void start_server() {
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
   
}


void send_data_to_server() {
    // Set up the UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cout << "Could not create socket" << std::endl;
        return;
    }

    // Set up the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.103");
    server_addr.sin_port = htons(12345);

    // Generate and send data to the server
    while (true) {
        // Generate random values for the four parameters
        int time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        double temperature = rand() % 30 + 10;
        double humidity = rand() % 50 + 30;
        double light = rand() % 10000 + 1000;

        // Convert the data to JSON format
        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("time");
        writer.Int(time);
        writer.Key("temperature");
        writer.Double(temperature);
        writer.Key("humidity");
        writer.Double(humidity);
        writer.Key("light");
        writer.Double(light);
        writer.EndObject();
        std::string json_str = s.GetString();

        // Send the data to the server
        int send_bytes = sendto(sock, json_str.c_str(), json_str.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (send_bytes < 0) {
            std::cout << "Could not send data" << std::endl;
        }

        // Wait for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(clientSock);
    close(sock);
}

int main() {
    // 创建一个线程用于发送数据到服务器
    std::thread data_sender_thread(send_data_to_server);

    // 启动服务器
    start_server();

    // 等待数据发送线程结束
    data_sender_thread.join();

    return 0;
}
