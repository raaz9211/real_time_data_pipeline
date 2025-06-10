#include <thread>
#include <iostream>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

#include "MetricsCollector.hpp"

void start_metrics_server(int port = 8080) {
    std::thread([](int port) {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            std::cerr << "Socket creation failed." << std::endl;
            return;
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            std::cerr << "Bind failed\n";
            return;
        }

        listen(server_fd, 3);
        std::cout << "Metrics server listening on http://localhost:" << port << "/metrics\n";

        while (true) {
            int addrlen = sizeof(address);
            int client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
            if (client_fd < 0) {
                std::cerr << "Accept failed.\n";
                continue;
            }

            char buffer[1024] = {0};
            read(client_fd, buffer, 1024);
            std::string request(buffer);

            if (request.find("GET /metrics") != std::string::npos) {
                std::string body = MetricsCollector::instance().format_prometheus();
                std::ostringstream response;
                response << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
                        << body.size() << "\r\n\r\n" << body;
                send(client_fd, response.str().c_str(), response.str().size(), 0);
            } else {
                const char *not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
                send(client_fd, not_found, strlen(not_found), 0);
            }
            close(client_fd);
        }
    }, port).detach();
}
