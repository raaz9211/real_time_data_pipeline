#include "TrimAndTagStrategy.hpp"

std::string TrimAndTagStrategy::process(const std::string &line)
{
    std::string trimmed = line;
    trimmed.erase(trimmed.find_last_not_of(" \n\r\t") + 1);
    trimmed.erase(0, trimmed.find_first_not_of(" \n\r\t"));
    return "[Processed] " + trimmed;
}