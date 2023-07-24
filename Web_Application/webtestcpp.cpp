#include <iostream>
#include <libwebsockets.h>
#include <string>
#include <thread>
#include <random>

// WebSocket服务器端口
#define WEBSOCKET_PORT 8080

// WebSocket协议
static const struct lws_protocols protocols[] = {
    { "example-protocol", callback_example, 0, 0 },
    { NULL, NULL, 0, 0 } // 结束标记
};

// WebSocket回调函数
static int callback_example(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED: // WebSocket连接建立时触发
            std::cout << "WebSocket connection established" << std::endl;
            break;

        case LWS_CALLBACK_CLOSED: // WebSocket连接关闭时触发
            std::cout << "WebSocket connection closed" << std::endl;
            break;

        default:
            break;
    }

    return 0;
}

// 发送随机数据给客户端
void sendRandomData(struct lws *wsi)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    while (lws_service(wsi, 0) >= 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int randomData = dist(gen);

        // 将随机数据发送给客户端
        std::string message = std::to_string(randomData);
        lws_write(wsi, reinterpret_cast<unsigned char*>(const_cast<char*>(message.c_str())), message.length(), LWS_WRITE_TEXT);
    }
}

int main()
{
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = WEBSOCKET_PORT;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    struct lws_context *context = lws_create_context(&info);

    if (!context) {
        std::cerr << "WebSocket server creation failed" << std::endl;
        return -1;
    }

    std::thread sendDataThread(sendRandomData, lws_client_connect(context, "localhost", WEBSOCKET_PORT, 0, "/", "localhost", nullptr, nullptr, -1));

    // 进入WebSocket事件循环
    while (true) {
        lws_service(context, 50);
    }

    // 清理资源
    lws_context_destroy(context);

    return 0;
}
