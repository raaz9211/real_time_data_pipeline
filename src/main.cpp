// main.cpp
#include <iostream>
#include <memory>
#include <filesystem>
#include <thread>
#include <atomic>

#include "FileDataSource.hpp"
#include "DataProcessor.hpp"
#include "Logger.hpp"
#include "OutputWriter.hpp"

int main()
{
    Logger::instance().log(LogLevel::INFO, "Real-Time Data Pipeline started.");
    std::cout << "Real-Time Data Pipeline started." << std::endl;

    ThreadSafeQueue<std::string> raw_queue;
    ThreadSafeQueue<std::string> processed_queue;
    std::atomic<bool> producer_done = false;
    std::atomic<bool> processing_done = false;

    auto file_path = std::filesystem::path("../data/input.txt");
    std::unique_ptr<DataSource> fileReader = std::make_unique<FileDataSource>(file_path);
    DataProcessor dataProcessor;
    OutputWriter outputWriter("../data/output.txt");
    Logger::instance().set_log_file("../logs/output.log");

    // Producer thread
    std::thread producer([&]()
                         {
                             try
                             {
                                 Logger::instance().log(LogLevel::DEBUG, "Producer thread started.");
                                 fileReader->fetch_data(raw_queue);
                                 Logger::instance().log(LogLevel::DEBUG, "Producer thread finished.");
                             }
                             catch (const std::exception &e)
                             {
                                 Logger::instance().log(LogLevel::ERROR, (std::string) "Producer exception: " + e.what());
                             }
                             producer_done = true; // ✅ Set this to notify consumer that no more data is coming
                         });

    // Consumer thread
    std::thread consume([&]()
                        {
                            try
                            {
                                Logger::instance().log(LogLevel::DEBUG, "Consume thread started.");
                                dataProcessor.consume(raw_queue, producer_done, processed_queue);
                                Logger::instance().log(LogLevel::DEBUG, "Consume thread finished.");
                            }
                            catch (const std::exception &e)
                            {
                                Logger::instance().log(LogLevel::ERROR, (std::string) "Consume exception: " + e.what());
                            }
                            processing_done = true; // Mark processing as complete
                        });

    std::thread collector([&]()
                          {
                            Logger::instance().log(LogLevel::DEBUG, "Collector thread started.");
                            size_t count = 0;
                            while (!processing_done || !processed_queue.empty())
                            {
                                std::string processed;
                                if (processed_queue.try_pop(processed))
                                {
                                    outputWriter.write(processed);
                                    count++;
                                }
                                else
                                {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                                }
                            }
                            Logger::instance().log(LogLevel::INFO, "Total processed lines written: " + std::to_string(count));
                            Logger::instance().log(LogLevel::DEBUG, "Collector thread finished."); });

    producer.join();
    consume.join();
    collector.join();

    std::cout << "Data processing complete. Output written to ../data/output.txt" << std::endl;
    Logger::instance().log(LogLevel::INFO, "Data processing complete. Output written to ../data/output.txt\n");

    return 0;
}