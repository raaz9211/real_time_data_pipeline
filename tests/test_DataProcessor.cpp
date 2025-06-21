#include <gtest/gtest.h>
#include <atomic>

#include "RingBufferQueue.hpp"
#include "DataProcessor.hpp"
#include "TrimAndTagStrategy.hpp"
#include "UpperCaseStrategy.hpp"

TEST(TrimAndTagStrategyTest, ProcessesLineCorrectly)
{
    TrimAndTagStrategy trimAndTagStrategy;
    std::string input = "   Hello World!  \n";
    std::string expected = "[Processed] Hello World!";
    EXPECT_EQ(trimAndTagStrategy.process(input), expected);
}


TEST(UpperCaseStrategyTest, ConvertsToUpperCase)
{
    UpperCaseStrategy upperCaseStrategy;
    std::string input = "hello";
    std::string expected = "HELLO";
    EXPECT_EQ(upperCaseStrategy.process(input), expected);
}

TEST(DataProcessorTrimAndTagStrategyTest, UsesStrategyCorrectly)
{
    RingBufferQueue<std::string> input(1024);
    RingBufferQueue<std::string> output(1024);
    std::atomic<bool> done(false);

    input.push(" hello ");
    done = true;

    std::unique_ptr<DataProcessor> dataProcessor = std::make_unique<DataProcessor>(std::make_unique<TrimAndTagStrategy>());
    dataProcessor->consume(input, done, output);

    std::string result;
    ASSERT_TRUE(output.try_pop(result));
    EXPECT_EQ(result, "[Processed] hello");

}

TEST(DataProcessorUpperCaseStrategyTTest, UsesStrategyCorrectly)
{
    RingBufferQueue<std::string> input(1024);
    RingBufferQueue<std::string> output(1024);
    std::atomic<bool> done(false);

    input.push("hello");
    done = true;

    std::unique_ptr<DataProcessor> dataProcessor = std::make_unique<DataProcessor>(std::make_unique<UpperCaseStrategy>());
    dataProcessor->consume(input, done, output);

    std::string result;
    ASSERT_TRUE(output.try_pop(result));
    EXPECT_EQ(result, "HELLO");

}
