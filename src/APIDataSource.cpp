#include <stdexcept>
#include <curl/curl.h>
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

#include "APIDataSource.hpp"
#include "Logger.hpp"

APIDataSource::APIDataSource(const std::string &url) : url_(url) {}

size_t WriteCallback(char *ptr, size_t size, size_t memb, void *userdata)
{
    size_t totalSize = size * memb;
    auto *response = static_cast<std::string *>(userdata);
    response->append(ptr, totalSize);
    return totalSize;
}

void APIDataSource::fetch_data(RingBufferQueue<std::string> &queue, std::atomic<bool> &stop_flag)
{
    constexpr int max_retries = 5;
    int retry_delay_ms = 500; // initial delay for backoff

    for (int attempt = 1; attempt <= max_retries; ++attempt)
    {
        if (stop_flag.load())
        {
            Logger::instance().log(LogLevel::INFO, "Fetch stopped by stop_flag.");
            return;
        }

        CURL *curl = curl_easy_init();
        if (!curl)
        {
            Logger::instance().log(LogLevel::ERROR, "Failed to initialize libcurl");
            throw std::runtime_error("Failed to initialize libcurl");
        }

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            try
            {
                // Parse JSON array and push strings to queue

                auto jsonData = nlohmann::json::parse(response);

                for (const auto &item : jsonData)
                {
                    queue.push(item.dump()); // Push the JSON object as a string
                }
                Logger::instance().log(LogLevel::INFO, "API fetch successful.");
                return; // success, exit function
            }
            catch (const std::exception &e)
            {
                Logger::instance().log(LogLevel::ERROR, std::string("JSON parse error: ") + e.what());
                throw std::runtime_error("JSON parse error: " + std::string(e.what()));
            }
        }
        else
        {
            Logger::instance().log(LogLevel::WARNING, "API fetch failed on attempt " + std::to_string(attempt) + ": " + std::string(curl_easy_strerror(res)));
            if (attempt == max_retries)
            {
                Logger::instance().log(LogLevel::ERROR, "Max retries reached, aborting API fetch.");
                throw std::runtime_error("Max retries reached for API fetch: " + std::string(curl_easy_strerror(res)));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
            retry_delay_ms *= 2; // exponential backoff
        }

        curl_easy_cleanup(curl);
    }
}
