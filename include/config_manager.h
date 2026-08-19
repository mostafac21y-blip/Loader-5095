#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct LoaderConfig {
    // Display settings
    bool fullscreen;
    int target_display;
    std::vector<int> multi_display_mode; // indices of displays to use
    
    // Graphics settings
    int fps_limit;
    bool vsync;
    float scale;
    
    // Colors
    struct {
        float r, g, b, a;
    } background_color;
    
    // Animation
    float animation_speed;
    std::string loader_style; // "spinner", "progress", "pulse", "wave"
    
    // Update settings
    bool auto_update;
    std::string update_url;
    std::string version;
};

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();
    
    // Load configuration from file
    bool load_config(const std::string& config_file);
    
    // Save configuration to file
    bool save_config(const std::string& config_file);
    
    // Get configuration
    const LoaderConfig& get_config() const;
    LoaderConfig& get_config_mut();
    
    // Reset to defaults
    void reset_to_defaults();
    
    // Validate configuration
    bool validate() const;
    
private:
    LoaderConfig config;
    
    // Set default values
    void set_defaults();
    
    // Convert to/from JSON
    json to_json() const;
    void from_json(const json& j);
};
