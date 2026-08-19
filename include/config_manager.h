#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ColorRGB {
    float r, g, b, a;
};

struct LoaderConfig {
    // Display settings
    bool fullscreen;
    int target_display;
    std::vector<int> multi_display_mode;
    float scale;
    
    // Graphics settings
    int fps_limit;
    bool vsync;
    ColorRGB background_color;
    
    // Animation settings
    float animation_speed;
    std::string loader_style;
    
    // Effects settings
    bool effects_enabled;
    std::string effect_type;
    ColorRGB effect_color;
    
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
    
    // Helper methods
    void set_defaults();
    json to_json() const;
    void from_json(const json& j);
};
