#pragma once

#include <vector>
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include "display_manager.h"
#include "graphics.h"

class Loader {
public:
    Loader();
    ~Loader();

    // Initialize loader
    bool initialize();

    // Run loader animation
    void run();

    // Stop loader
    void stop();

    // Configuration
    void set_fullscreen(bool fullscreen);
    void set_target_displays(const std::vector<int>& displays);
    void set_animation_speed(float speed);
    void set_background_color(float r, float g, float b, float a = 1.0f);

private:
    bool is_running;
    bool fullscreen_mode;
    float animation_speed;
    float background_color[4];
    std::vector<int> target_displays;

    std::unique_ptr<DisplayManager> display_manager;
    std::vector<std::unique_ptr<Graphics>> graphics_contexts;
    std::vector<SDL_Window*> windows;

    // Event handling
    void handle_events();

    // Update animation state
    void update(float delta_time);

    // Render to all displays
    void render();

    // Create windows for target displays
    bool create_windows();

    // Cleanup resources
    void cleanup();
};
