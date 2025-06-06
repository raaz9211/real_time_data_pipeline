#pragma once

#include <string>
#include <unordered_map>
#include <json.hpp>

class ConfigManager
{
public:
    explicit ConfigManager(const std::string &configFilePath);

    std::string getDataSourceType() const;
    void validate() const;
    std::string getFilePath() const;
    std::string getApiUrl() const;
    std::string getOutputPath() const;
    std::string getLogFilePath() const;
    bool isTimestampEnabled() const;

private:
    nlohmann::json configData;
};