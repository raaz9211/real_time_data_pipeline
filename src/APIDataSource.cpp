#include <stdexcept>
#include <curl/curl.h>
#include <iostream>
#include <json.hpp>

#include "APIDataSource.hpp"

APIDataSource::APIDataSource(const std::string &url) : url_(url) {}

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t totalSize = size * nmemb;
    auto *response = static_cast<std::string *>(userdata);
    response->append(ptr, totalSize);
    return totalSize;
}

void APIDataSource::fetch_data(ThreadSafeQueue<std::string> &queue)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        throw std::runtime_error("Failed to initialize libcurl");
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("libcurl request failed: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);


    // Parse JSON array and push strings to queue

    auto jsonData = nlohmann::json::parse(response);

    for(const auto &item : jsonData) {
        queue.push(item.dump());  // Push the JSON object as a string

    }

}

