#pragma once

#include <csignal>
#include <atomic>

class SignalHandler {
public:
    static void setup(std::atomic<bool> &stop_signal);
private:
    static void handle_signal(int signal);
    static std::atomic<bool> *stop_flag;
};