#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <ixwebsocket/IXWebSocket.h>

class WebSocketClient {
public:
    explicit  WebSocketClient(const std::string& url);
    ~WebSocketClient();

    void start();
    void stop();
    void send(const std::string& message);

private:
    void run();

    std::string url_;
    ix::WebSocket socket_;
    std::thread thread_;
    std::atomic<bool> running{false};

    std::queue<std::string> message_queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
};
