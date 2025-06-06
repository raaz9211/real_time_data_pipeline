#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <thread>
#include <atomic>

#include "ConfigManager.hpp"
#include "DataSourceFactory.hpp"
#include "ThreadSafeQueue.hpp"
#include "DataProcessor.hpp"
#include "TrimAndTagStrategy.hpp"
#include "OutputWriter.hpp"
#include "OutputWriter.hpp"
#include "FileDataSource.hpp"


namespace fs = std::filesystem;

TEST(FullPipelineTest, ProcessesFileEndToEnd) {

    // 1) Arrage: write a small input file
    const std::string inputFile = "../tests/temp_input.txt";
    const std::string outputFile = "../tests/temp_output.txt";

    {
        std::ofstream out(inputFile);
        out << "foo\nbar\nbaz";
    }

    // 2) Create DataSource, Processor, and OutputWriter
    auto source = std::make_unique<FileDataSource>(inputFile);
    ThreadSafeQueue<std::string> rawQueue;

    std::atomic<bool> producerDone = false;

    // Producer thread: read from file into rawQueue
    std::thread producer([&]() {
        // Producer thread: read from file into rawQueue
        source->fetch_data(rawQueue);
        producerDone = true;
    });

    DataProcessor processor(std::make_unique<TrimAndTagStrategy>());
    ThreadSafeQueue<std::string> processedQueue;


    std::thread consumer([&]() {
        // Consumer thread: process rawQueue into processedQueue
        processor.consume(rawQueue, producerDone, processedQueue);
    });


    // 3) In the main thread: write processedQueue to outputFile
    OutputWriter writer(outputFile, OutputWriter::OutputMode::File, false, false);


    // Wait for producer and consumer to finish
    producer.join();
    consumer.join();

    // Pop everything from processedQueue and write to disk
    std::string line;
    while (processedQueue.try_pop(line)) 
    {
        writer.write(line);
    }
    

    // 4) Assert: read outputFile and verify contents
    std::ifstream in(outputFile);
    std::vector<std::string> lines;
    while ( (std::getline(in, line)))
    {
        lines.push_back(line);
    }
    
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "[Processed] foo");
    EXPECT_EQ(lines[1], "[Processed] bar");
    EXPECT_EQ(lines[2], "[Processed] baz");

    // Cleanup
    fs::remove(inputFile);
    fs::remove(outputFile);
}