#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace Utils {
    inline std::string current_timestamp() {
        using namespace std::chrono;
        auto now = system_clock::now();
        std::time_t now_time = system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&now_time);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}
