#pragma once

#include <memory>
#include <string>

#include "DataSource.hpp"
#include "ConfigManager.hpp"


class DataSourceFactory {

public:
    static std::unique_ptr<DataSource> create(const std::string &type, const ConfigManager &config);
};