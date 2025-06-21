#include "DatabaseWriter.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Utils.hpp"

DatabaseWriter::DatabaseWriter(const std::string &db_path) : db(nullptr) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Can't open database " << db_path << std::endl;
        db = nullptr;
    } else {
        create_table_if_not_exists();
    }
}

DatabaseWriter::~DatabaseWriter() {
    if (db) {
        sqlite3_close(db);
    }
}

void DatabaseWriter::create_table_if_not_exists() {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS processed_records ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "processed_at TEXT NOT NULL, "
        "processed_data TEXT NOT NULL);";

    char *err_msg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "Create table failed: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
}

void DatabaseWriter::insert_record(const std::string &processed) {
    std::lock_guard<std::mutex> lock(mtx);

    std::string sql = "INSERT INTO processed_records (processed_at, processed_data) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string timestamp = Utils::current_timestamp();

    sqlite3_bind_text(stmt, 1, timestamp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, processed.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
