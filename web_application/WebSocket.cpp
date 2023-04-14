#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std::chrono_literals;

int main()
{
    // 初始化Winsock2
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock2!" << std::endl;
        return 1;
    }

    // 创建Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        WSACleanup();
        return 1;
    }

    // 绑定Socket到本地地址和端口
    SOCKADDR_IN addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);
    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 监听连接请求
    if (listen(sock, 1) == SOCKET_ERROR) {
        std::cerr << "Failed to listen socket!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 等待连接
    std::cout << "Waiting for connection..." << std::endl;
    SOCKADDR_IN clientAddr = { 0 };
    int addrLen = sizeof(clientAddr);
    SOCKET clientSock = accept(sock, (SOCKADDR*)&clientAddr, &addrLen);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Failed to accept connection!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // 实时输出参数到网络
    int i = 0;
    while (true) {
        std::string data = std::to_string(i);
        if (send(clientSock, data.c_str(), data.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send data!" << std::endl;
            break;
        }
        std::this_thread::sleep_for(1s); // 等待1秒
        i++;
    }

    // 关闭连接和Socket
    closesocket(clientSock);
    closesocket(sock);
    WSACleanup();

    return 0;
}