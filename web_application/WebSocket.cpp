#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std::chrono_literals;

int main()
{
    // 创建Socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        std::cerr << "Failed to create socket!" << std::endl;
        return 1;
    }

    // 绑定Socket到本地地址和端口
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to bind socket!" << std::endl;
        close(sock);
        return 1;
    }

    // 监听连接请求
    if (listen(sock, 1) == -1) {
        std::cerr << "Failed to listen socket!" << std::endl;
        close(sock);
        return 1;
    }

    // 等待连接
    std::cout << "Waiting for connection..." << std::endl;
    struct sockaddr_in clientAddr = { 0 };
    socklen_t addrLen = sizeof(clientAddr);
    int clientSock = accept(sock, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSock == -1) {
        std::cerr << "Failed to accept connection!" << std::endl;
        close(sock);
        return 1;
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
        std::this_thread::sleep_for(1s); // 等待1秒
        i++;
    }

    // 关闭连接和Socket
    close(clientSock);
    close(sock);

    return 0;
}
