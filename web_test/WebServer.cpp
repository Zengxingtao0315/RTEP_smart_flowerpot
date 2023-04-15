#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a socket for the server to listen on
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Bind the socket to a port
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.1.103");
    server_address.sin_port = htons(12345);
    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket to port" << std::endl;
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server is listening on port 12345" << std::endl;

    // Accept incoming connections
    sockaddr_in client_address = {
    .sin_family = AF_INET,
    .sin_port = htons(12345),
    .sin_addr.s_addr = inet_addr("192.168.0.208"),
    .sin_zero = {0}
	};
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size);
    if (client_socket == -1) {
        std::cerr << "Failed to accept incoming connection" << std::endl;
        close(server_socket);
        return 1;
    }else{
		std::cout<<"success connection"<<std::endl;
		
	}

    std::cout << "Client connected" << std::endl;

    // Send updated data to the client every second
    while (true) {
        std::string data = "{\"time\": \"12:00\", \"temperature\": 23.5, \"humidity\": 60, \"light\": 100}";

        ssize_t bytes_sent = send(client_socket, data.c_str(), data.size(), 0);
        if (bytes_sent == -1) {
            std::cerr << "Failed to send data to client" << std::endl;
            close(client_socket);
            close(server_socket);
            return 1;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    

    return 0;
}
