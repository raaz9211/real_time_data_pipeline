#include <sstream>

#include "MetricsCollector.hpp"


MetricsCollector::MetricsCollector()
    : processed_count(0), error_count(0), total_latency_ms(0), latency_samples(0),
      raw_queue_size(0), processed_queue_size(0) {
}

MetricsCollector &MetricsCollector::instance() {
    static MetricsCollector instance;
    return instance;
}

void MetricsCollector::increment_processed() {
    processed_count++;
}

void MetricsCollector::increment_errors() {
    error_count++;
}

void MetricsCollector::add_latency(double ms) {
    total_latency_ms = total_latency_ms + ms;
    latency_samples++;
}

void MetricsCollector::set_queue_sizes(size_t row, size_t processed) {
    std::lock_guard<std::mutex> lock(mtx);
    raw_queue_size = row;
    processed_queue_size = processed;
}

std::string MetricsCollector::format_prometheus() const {
    std::ostringstream oss;
    oss << "# HELP processed_count Total items processed\n";
    oss << "# TYPE processed_count counter\n";
    oss << "processed_count " << processed_count << "\n";

    oss << "# HELP error_count Total errors encountered\n";
    oss << "# TYPE error_count counter\n";
    oss << "error_count " << error_count << "\n";

    double avg_latency = latency_samples > 0 ? total_latency_ms / latency_samples : 0.0;
    oss << "# HELP avg_latency_ms Average latency per item\n";
    oss << "# TYPE avg_latency_ms gauge\n";
    oss << "avg_latency_ms " << avg_latency << "\n";

    std::lock_guard<std::mutex> lock(mtx);
    oss << "# HELP raw_queue_size Size of raw queue\n";
    oss << "# TYPE raw_queue_size gauge\n";
    oss << "raw_queue_size " << raw_queue_size << "\n";

    oss << "# HELP processed_queue_size Size of processed queue\n";
    oss << "# TYPE processed_queue_size gauge\n";
    oss << "processed_queue_size " << processed_queue_size << "\n";

    return oss.str();
}
