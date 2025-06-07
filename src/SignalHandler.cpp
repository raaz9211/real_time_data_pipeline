#include <iostream>

#include "SignalHandler.hpp"

std::atomic<bool> *SignalHandler::stop_flag = nullptr;

void SignalHandler::setup(std::atomic<bool> &stop_signal)
{
    stop_flag = &stop_signal;
    std::signal(SIGINT, handle_signal); // CTRL + C
    std::signal(SIGINT, handle_signal); // Termination request
}

void SignalHandler::handle_signal(int signals)
{
    if (stop_flag)
    {
        *stop_flag = true;
        std::cerr << "\n[SignalHandler] Caught signal " << signal << ", shutting down gracefully...\n";
    }
}