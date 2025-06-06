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

void ConfigManager::validate() const
{
    if (!getFilePath().empty() && !std::filesystem::exists(getFilePath()))
    {
        throw std::runtime_error("Invalid file path: " + getFilePath());
    }
    // You can also validate API URL here if applicable
    if (!getApiUrl().empty() && getApiUrl().find("http") != 0)
    {
        throw std::runtime_error("Invalid API URL: " + getApiUrl());
    }
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
