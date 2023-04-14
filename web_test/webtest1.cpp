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

int main() {
    // Set up the UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cout << "Could not create socket" << std::endl;
        return 1;
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

    return 0;
}
