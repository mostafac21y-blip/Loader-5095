#pragma once

#include <string>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct UpdateInfo {
    std::string version;
    std::string download_url;
    std::string changelog;
    bool mandatory;
    size_t file_size;
    std::string checksum;
};

class UpdateManager {
public:
    UpdateManager();
    ~UpdateManager();
    
    // Check for updates from remote server
    bool check_for_updates(const std::string& update_url);
    
    // Get available updates
    const std::vector<UpdateInfo>& get_available_updates() const;
    
    // Download update
    bool download_update(const UpdateInfo& update, 
                        const std::string& destination,
                        std::function<void(float)> progress_callback = nullptr);
    
    // Install update
    bool install_update(const std::string& update_file);
    
    // Get current version
    const std::string& get_current_version() const;
    
    // Check if update available
    bool is_update_available() const;
    
private:
    std::string current_version;
    std::vector<UpdateInfo> available_updates;
    
    // Helper methods
    bool parse_update_response(const std::string& response);
    bool verify_checksum(const std::string& file, const std::string& checksum);
};
