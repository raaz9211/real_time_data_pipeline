#include "ProcessingStrategy.hpp"
#include <algorithm>

class UpperCaseStrategy : public ProcessingStrategy
{
public:
    std::string process(const std::string &line);
};