#include "CommandLineParser.hpp"
#include <sstream>
#include <algorithm>  

CommandLineParser::CommandLineParser(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        auto eq_pos = arg.find('=');

        if (eq_pos != std::string::npos)
        {
            std::string key = arg.substr(0, eq_pos);
            std::string value = arg.substr(eq_pos + 1);
            options_[key] = value;
        }
        else
        {
            flags_.push_back(arg);
        }
    }
}

std::string CommandLineParser::get_option(const std::string &option, const std::string &default_value) const
{
    return options_.count(option) ? options_.at(option) : default_value;
}

bool CommandLineParser::has_flag(const std::string &flag) const
{
    return std::find(flags_.begin(), flags_.end(), flag) != flags_.end();
}