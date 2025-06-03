#include "UpperCaseStrategy.hpp"
#include <algorithm>

std::string UpperCaseStrategy::process(const std::string &line)
{
    std::string upper = line;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    return upper;
}