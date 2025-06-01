#include <fstream>
#include <stdexcept>

#include "ConfigManager.hpp"

ConfigManager::ConfigManager(const std::string &configFilePath)
{
    std::ifstream file(configFilePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open config file: " + configFilePath);
    }
    file >> configData;
}

std::string ConfigManager::getDataSourceType() const
{
    return configData.value("data_source", "file");
}

std::string ConfigManager::getFilePath() const
{
    return configData.value("file_path", "../data/input.txt");
}

std::string ConfigManager::getApiUrl() const
{
    return configData.value("api_url", "");
}

std::string ConfigManager::getOutputPath() const
{
    return configData.value("output_path", "../data/output.txt");
}

std::string ConfigManager::getLogFilePath() const
{
    return configData.value("log_file_path", "../logs/output.log");
}

bool ConfigManager::isTimestampEnabled() const
{
    return configData.value("enable_timestamp", false);
}

