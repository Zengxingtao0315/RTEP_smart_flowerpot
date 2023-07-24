#include <iostream>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <string>
#include <thread>  // 添加缺少的头文件
#include <chrono>  // 添加缺少的头文件


// 全局变量，模拟实时更新的数据
int realtimeData = 0;

void onRequest(struct evhttp_request* req, void* arg) {
    struct evbuffer* buf = evbuffer_new();
    if (!buf) {
        std::cerr << "Failed to create response buffer." << std::endl;
        return;
    }

    // 设置HTTP响应头，指定数据格式为text/event-stream
    evhttp_add_header(req->output_headers, "Content-Type", "text/event-stream");

    // 构建数据
    std::string eventData = "data: " + std::to_string(realtimeData) + "\n\n";

    // 将数据写入响应缓冲区
    evbuffer_add(buf, eventData.c_str(), eventData.size());

    // 发送响应
    evhttp_send_reply(req, HTTP_OK, "OK", buf);

    // 释放资源
    evbuffer_free(buf);
}

int main() {
    struct event_base* base = event_base_new();
    if (!base) {
        std::cerr << "Failed to create event base." << std::endl;
        return -1;
    }

    struct evhttp* http = evhttp_new(base);
    if (!http) {
        std::cerr << "Failed to create evhttp." << std::endl;
        return -1;
    }

    evhttp_bind_socket(http, "0.0.0.0", 8080); // 监听地址和端口

    // 设置请求处理回调函数
    evhttp_set_gencb(http, onRequest, nullptr);

    // 模拟数据更新
    event_base_loopexit(base, nullptr); // 停止事件循环
    while (true) {
        realtimeData++; // 更新数据
        event_base_loop(base, EVLOOP_NONBLOCK); // 非阻塞模式运行事件循环
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒更新一次数据
    }

    evhttp_free(http);
    event_base_free(base);
    return 0;
}
