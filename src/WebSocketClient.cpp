#include "WebSocketClient.hpp"
#include <iostream>

WebSocketClient::WebSocketClient(const std::string& url) : url_(url) {}

WebSocketClient::~WebSocketClient() {
    stop();
}

void WebSocketClient::start() {
    running = true;
    socket_.setUrl(url_);

    // Only use supported callbacks
    socket_.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            std::cout << "[WebSocket] Received: " << msg->str << std::endl;
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cerr << "[WebSocket] Error: " << msg->errorInfo.reason << std::endl;
        }
    });

    socket_.start();
    thread_ = std::thread(&WebSocketClient::run, this);
}

void WebSocketClient::stop() {
    running = false;
    cv_.notify_all();

    if (thread_.joinable()) {
        thread_.join();
    }

    socket_.stop();
}

void WebSocketClient::send(const std::string& message) {
    {
        std::unique_lock<std::mutex> lock(mtx_);
        message_queue_.push(message);
    }
}

void WebSocketClient::run() {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() { return !message_queue_.empty() || !running; });

        while (!message_queue_.empty()) {
            std::string msg = std::move(message_queue_.front());
            message_queue_.pop();
            lock.unlock();

            socket_.send(msg);

            lock.lock();
        }
    }
}
