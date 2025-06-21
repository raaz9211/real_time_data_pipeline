#pragma once

#include <string>
#include <nlohmann/json.hpp>

class ConfigManager
{
public:
    explicit ConfigManager(const std::string &configFilePath);

    void validate() const;
    std::string getDataSourceType() const;
    std::string getFilePath() const;
    std::string getApiUrl() const;
    std::string getOutputPath() const;
    std::string getOutputDBPath() const;
    std::string getLogFilePath() const;
    bool isTimestampEnabled() const;

private:
    nlohmann::json configData;
};