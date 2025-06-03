#pragma once
#include <string>

class ProcessingStrategy {
public:
    virtual std::string process(const std::string &line) = 0;
    virtual ~ProcessingStrategy() = default;
};