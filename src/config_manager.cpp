#include "config_manager.h"
#include <iostream>
#include <fstream>

ConfigManager::ConfigManager() {
    set_defaults();
}

ConfigManager::~ConfigManager() {}

bool ConfigManager::load_config(const std::string& config_file) {
    try {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            std::cout << "Config file not found, using defaults: " << config_file << std::endl;
            return false;
        }

        json j;
        file >> j;
        from_json(j);

        std::cout << "Configuration loaded successfully" << std::endl;
        return validate();
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        set_defaults();
        return false;
    }
}

bool ConfigManager::save_config(const std::string& config_file) {
    try {
        std::ofstream file(config_file);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file for writing" << std::endl;
            return false;
        }

        json j = to_json();
        file << j.dump(4);
        file.close();

        std::cout << "Configuration saved successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving config: " << e.what() << std::endl;
        return false;
    }
}

const LoaderConfig& ConfigManager::get_config() const {
    return config;
}

LoaderConfig& ConfigManager::get_config_mut() {
    return config;
}

void ConfigManager::reset_to_defaults() {
    set_defaults();
}

bool ConfigManager::validate() const {
    if (config.fps_limit < 1 || config.fps_limit > 240) {
        std::cerr << "Invalid FPS limit: " << config.fps_limit << std::endl;
        return false;
    }

    if (config.background_color.r < 0 || config.background_color.r > 1 ||
        config.background_color.g < 0 || config.background_color.g > 1 ||
        config.background_color.b < 0 || config.background_color.b > 1 ||
        config.background_color.a < 0 || config.background_color.a > 1) {
        std::cerr << "Invalid background color values" << std::endl;
        return false;
    }

    return true;
}

void ConfigManager::set_defaults() {
    config.fullscreen = true;
    config.target_display = 0;
    config.multi_display_mode = {0};
    config.fps_limit = 60;
    config.vsync = true;
    config.scale = 1.0f;
    
    config.background_color.r = 0.1f;
    config.background_color.g = 0.1f;
    config.background_color.b = 0.1f;
    config.background_color.a = 1.0f;
    
    config.animation_speed = 1.0f;
    config.loader_style = "spinner";
    config.auto_update = true;
    config.update_url = "https://api.github.com/repos/mostafac21y-blip/Loader-5095/releases/latest";
    config.version = "1.0.0";
}

json ConfigManager::to_json() const {
    json j;
    
    j["display"]["fullscreen"] = config.fullscreen;
    j["display"]["target_display"] = config.target_display;
    j["display"]["multi_display_mode"] = config.multi_display_mode;
    j["display"]["scale"] = config.scale;
    
    j["graphics"]["fps_limit"] = config.fps_limit;
    j["graphics"]["vsync"] = config.vsync;
    j["graphics"]["background_color"]["r"] = config.background_color.r;
    j["graphics"]["background_color"]["g"] = config.background_color.g;
    j["graphics"]["background_color"]["b"] = config.background_color.b;
    j["graphics"]["background_color"]["a"] = config.background_color.a;
    
    j["animation"]["speed"] = config.animation_speed;
    j["animation"]["style"] = config.loader_style;
    
    j["update"]["auto_update"] = config.auto_update;
    j["update"]["url"] = config.update_url;
    j["version"] = config.version;
    
    return j;
}

void ConfigManager::from_json(const json& j) {
    try {
        config.fullscreen = j.value("display", json::object()).value("fullscreen", true);
        config.target_display = j.value("display", json::object()).value("target_display", 0);
        config.fps_limit = j.value("graphics", json::object()).value("fps_limit", 60);
        config.vsync = j.value("graphics", json::object()).value("vsync", true);
        config.scale = j.value("display", json::object()).value("scale", 1.0f);
        
        auto bg = j.value("graphics", json::object()).value("background_color", json::object());
        config.background_color.r = bg.value("r", 0.1f);
        config.background_color.g = bg.value("g", 0.1f);
        config.background_color.b = bg.value("b", 0.1f);
        config.background_color.a = bg.value("a", 1.0f);
        
        config.animation_speed = j.value("animation", json::object()).value("speed", 1.0f);
        config.loader_style = j.value("animation", json::object()).value("style", "spinner");
        
        config.auto_update = j.value("update", json::object()).value("auto_update", true);
        config.update_url = j.value("update", json::object()).value("url", "");
        config.version = j.value("version", "1.0.0");
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON config: " << e.what() << std::endl;
        set_defaults();
    }
}
