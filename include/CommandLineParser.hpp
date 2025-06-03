#pragma once

#include <string>
#include <unordered_map>
#include <vector>


class CommandLineParser {
public:
    explicit CommandLineParser(int argc, char *argv[]);

    std::string get_option(const std::string &option, const std::string &default_value = "") const;
    bool has_flag(const std::string &flag) const;

private:
    std::unordered_map<std::string, std::string> options_;
    std::vector<std::string> flags_;
};