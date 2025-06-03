#include "ProcessingStrategy.hpp"

class TrimAndTagStrategy : public ProcessingStrategy
{
public:
    std::string process(const std::string &line);
};