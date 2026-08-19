#include "update_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <curl/curl.h>

// Callback for CURL data transfer
static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Callback for download progress
static int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                            curl_off_t ultotal, curl_off_t ulnow) {
    if (dltotal > 0) {
        float progress = static_cast<float>(dlnow) / static_cast<float>(dltotal);
        auto callback = static_cast<std::function<void(float)>*>(clientp);
        if (callback) {
            (*callback)(progress);
        }
    }
    return 0;
}

UpdateManager::UpdateManager() : current_version("1.0.0") {}

UpdateManager::~UpdateManager() {}

bool UpdateManager::check_for_updates(const std::string& update_url) {
    std::cout << "Checking for updates from: " << update_url << std::endl;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, update_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Failed to fetch updates: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return parse_update_response(response);
}

const std::vector<UpdateInfo>& UpdateManager::get_available_updates() const {
    return available_updates;
}

bool UpdateManager::download_update(const UpdateInfo& update,
                                    const std::string& destination,
                                    std::function<void(float)> progress_callback) {
    std::cout << "Downloading update: " << update.version << std::endl;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    std::ofstream output_file(destination, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open destination file: " << destination << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, update.download_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                    [](void* contents, size_t size, size_t nmemb, std::ofstream* file) {
                        file->write((char*)contents, size * nmemb);
                        return size * nmemb;
                    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output_file);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    if (progress_callback) {
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progress_callback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    }

    CURLcode res = curl_easy_perform(curl);
    output_file.close();
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    // Verify checksum
    if (!update.checksum.empty()) {
        if (!verify_checksum(destination, update.checksum)) {
            std::cerr << "Checksum verification failed" << std::endl;
            return false;
        }
    }

    std::cout << "Update downloaded successfully" << std::endl;
    return true;
}

bool UpdateManager::install_update(const std::string& update_file) {
    std::cout << "Installing update from: " << update_file << std::endl;
    // Implementation depends on platform and update mechanism
    // This is a placeholder for actual installation logic
    return true;
}

const std::string& UpdateManager::get_current_version() const {
    return current_version;
}

bool UpdateManager::is_update_available() const {
    return !available_updates.empty();
}

bool UpdateManager::parse_update_response(const std::string& response) {
    try {
        json j = json::parse(response);

        // Handle GitHub API response
        if (j.contains("tag_name") && j.contains("assets")) {
            UpdateInfo update;
            update.version = j["tag_name"].get<std::string>();
            update.changelog = j.value("body", "");
            update.mandatory = false;

            // Get download URL from assets
            auto assets = j["assets"];
            if (!assets.empty()) {
                update.download_url = assets[0]["browser_download_url"].get<std::string>();
                update.file_size = assets[0]["size"].get<size_t>();
            }

            if (!update.download_url.empty() && update.version != current_version) {
                available_updates.push_back(update);
                std::cout << "Update available: " << update.version << std::endl;
                return true;
            }
        }

        std::cout << "No updates available" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing update response: " << e.what() << std::endl;
        return false;
    }
}

bool UpdateManager::verify_checksum(const std::string& file, const std::string& checksum) {
    // Placeholder for checksum verification
    // In production, this should compute SHA256/MD5 of the file
    std::cout << "Checksum verification passed (placeholder)" << std::endl;
    return true;
}
