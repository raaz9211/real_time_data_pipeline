// main.cpp
#include <iostream>
#include <memory>
#include <filesystem>
#include <thread>
#include <atomic>

#include "DataSourceFactory.hpp"
#include "DataProcessor.hpp"
#include "Logger.hpp"
#include "OutputWriter.hpp"
#include "ConfigManager.hpp"
#include "TrimAndTagStrategy.hpp"
#include "CommandLineParser.hpp"
// #include "UpperCaseStrategy.hpp"

int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv);


    std::string config_path = parser.get_option("--config", "config.json");
    bool verbose = parser.has_flag("--verbose");
    
    ThreadSafeQueue<std::string> raw_queue;
    ThreadSafeQueue<std::string> processed_queue;
    std::atomic<bool> producer_done = false;
    std::atomic<bool> processing_done = false;

    ConfigManager config(config_path);

    auto file_path = std::filesystem::path(config.getFilePath());
    std::unique_ptr<DataSource> fileReader = DataSourceFactory::create(config.getDataSourceType(), config);
    std::unique_ptr<DataProcessor> dataProcessor = std::make_unique<DataProcessor>(std::make_unique<TrimAndTagStrategy>());
    OutputWriter outputWriter(config.getOutputPath(), OutputWriter::OutputMode::File, false, true);
    Logger::instance().set_log_file(config.getLogFilePath());

    Logger::instance().log(LogLevel::INFO, "Real-Time Data Pipeline started.");
    std::clog << "Real-Time Data Pipeline started." << std::endl;

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
                             producer_done = true; // Set this to notify consumer that no more data is coming
                         });

    // Consumer thread
    std::thread consume([&]()
                        {
                            try
                            {
                                Logger::instance().log(LogLevel::DEBUG, "Consume thread started.");
                                dataProcessor->consume(raw_queue, producer_done, processed_queue);
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

    std::clog << "Data processing complete. Output written to " + config.getOutputPath() << std::endl;
    Logger::instance().log(LogLevel::INFO, "Data processing complete. Output written to ../data/output.txt\n");

    return 0;
}