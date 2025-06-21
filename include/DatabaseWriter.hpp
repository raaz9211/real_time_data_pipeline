#pragma once

#include <string>
#include <sqlite3.h>
#include <mutex>

class DatabaseWriter {
public:
    explicit DatabaseWriter(const std::string &db_path);

    ~DatabaseWriter();

    void insert_record(const std::string &processed);

private:
    sqlite3 *db;
    std::mutex mtx;

    void create_table_if_not_exists();
};
