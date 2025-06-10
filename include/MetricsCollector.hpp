#pragma once

#include <atomic>
#include <mutex>
#include <string>

class MetricsCollector {
public:
    static MetricsCollector& instance();

    void increment_processed();
    void increment_errors();
    void add_latency(double ms);
    void set_queue_sizes(size_t row, size_t processed);

    std::string format_prometheus() const;

private:
    MetricsCollector();
    mutable std::mutex mtx;

    std::atomic<size_t> processed_count;
    std::atomic<size_t> error_count;
    std::atomic<double> total_latency_ms;
    std::atomic<size_t> latency_samples;

    size_t raw_queue_size;
    size_t processed_queue_size;
};
