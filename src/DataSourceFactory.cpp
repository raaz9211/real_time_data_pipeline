#include "DataSourceFactory.hpp"
#include "FileDataSource.hpp"
#include "APIDataSource.hpp" 

std::unique_ptr<DataSource> DataSourceFactory::create(const std::string &type, const ConfigManager &config)
{
    if (type == "file")
    {
        return std::make_unique<FileDataSource>(config.getFilePath());
    }
    else if (type == "api")
    {
        return std::make_unique<APIDataSource>(config.getApiUrl());
    }
    else
    {
        throw std::invalid_argument("Unknown data source type: " + type);
    }
}