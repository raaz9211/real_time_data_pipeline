// main.cpp
#include <iostream>
#include <memory>
#include <filesystem>
#include <thread>
#include <atomic>
#include <chrono>

#include "DataSourceFactory.hpp"
#include "DataProcessor.hpp"
#include "Logger.hpp"
#include "OutputWriter.hpp"
#include "ConfigManager.hpp"
#include "TrimAndTagStrategy.hpp"
// #include "UpperCaseStrategy.hpp"
#include "CommandLineParser.hpp"
#include "SignalHandler.hpp"
#include "ThreadPool.hpp"
#include "MetricsServer.hpp"
#include "MetricsCollector.hpp"

int main(int argc, char *argv[]) {
    try {
        CommandLineParser parser(argc, argv);

        std::string config_path = parser.get_option("--config", "../config/config.json");
        std::string metrics_server_port = parser.get_option("--port", "8080");
        bool verbose = parser.has_flag("--verbose");

        // Start metrics server
        start_metrics_server(std::stoi(metrics_server_port));

        ThreadSafeQueue<std::string> raw_queue;
        ThreadSafeQueue<std::string> processed_queue;
        std::atomic<bool> producer_done = false;
        std::atomic<bool> processing_done = false;
        std::atomic<bool> stop_flag = false;

        SignalHandler::setup(stop_flag);
        ConfigManager config(config_path);

        std::string data_source = parser.get_option("--data_source", config.getDataSourceType());

        std::unique_ptr<DataSource> fileReader = DataSourceFactory::create(data_source, config);
        std::unique_ptr<DataProcessor> dataProcessor = std::make_unique<DataProcessor>(
            std::make_unique<TrimAndTagStrategy>());
        OutputWriter outputWriter(config.getOutputPath(), OutputWriter::OutputMode::File, false, true);
        Logger::instance().set_log_file(config.getLogFilePath());

        Logger::instance().log(LogLevel::INFO, "Real-Time Data Pipeline started.");
        std::clog << "Real-Time Data Pipeline started." << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        // Create a thread pool with number of threads equal to hardware concurrency
        ThreadPool pool(std::thread::hardware_concurrency());

        // Producer Task
        pool.submit([&]() {
            try {
                Logger::instance().log(LogLevel::DEBUG, "Producer thread started.");
                fileReader->fetch_data(raw_queue, stop_flag);
                Logger::instance().log(LogLevel::DEBUG, "Producer thread finished.");
            } catch (const std::exception &e) {
                Logger::instance().log(LogLevel::ERROR, (std::string) "Producer exception: " + e.what());
                MetricsCollector::instance().increment_errors();
            }
            producer_done = true; // Set this to notify consumer that no more data is coming
        });

        // Consumer Task
        pool.submit([&]() {
            try {
                Logger::instance().log(LogLevel::DEBUG, "Consume thread started.");
                dataProcessor->consume(raw_queue, producer_done, processed_queue);
                Logger::instance().log(LogLevel::DEBUG, "Consume thread finished.");
                MetricsCollector::instance().increment_errors();
            } catch (const std::exception &e) {
                Logger::instance().log(LogLevel::ERROR, (std::string) "Consume exception: " + e.what());
            }
            processing_done = true; // Mark processing as complete
        });

        std::atomic<size_t> count = 0;
        // Collector Task
        pool.submit([&]() {
            Logger::instance().log(LogLevel::DEBUG, "Collector thread started.");

            while (!processing_done || !processed_queue.empty()) {
                std::string processed;
                if (processed_queue.try_pop(processed)) {
                    auto t1 = std::chrono::high_resolution_clock::now();
                    outputWriter.write(processed);
                    auto t2 = std::chrono::high_resolution_clock::now();

                    double latency_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
                    MetricsCollector::instance().add_latency(latency_ms);
                    MetricsCollector::instance().increment_processed();

                    count++;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                // Update live queue size
                MetricsCollector::instance().set_queue_sizes(raw_queue.size(), processed_queue.size());
            }

            Logger::instance().log(LogLevel::INFO, "Total processed lines written: " + std::to_string(count));
            Logger::instance().log(LogLevel::DEBUG, "Collector thread finished.");
        });

        pool.shutdown();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::clog << "Processing took " << duration_ms << "ms" << std::endl;
        Logger::instance().log(LogLevel::INFO, "Processing took " + std::to_string(duration_ms) + " ms");

        double throughput = count.load() / (duration_ms / 1000.0); // records per second
        std::clog << "Throughput: " << throughput << " records/second\n";

        std::clog << "Data processing complete. Output written to " + config.getOutputPath() << std::endl;
        Logger::instance().log(LogLevel::INFO, "Data processing complete. Output written to ../data/output.txt\n");
    } catch (const std::exception &ex) {
        Logger::instance().log(LogLevel::ERROR, std::string("Unhandled exception: ") + ex.what());
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
