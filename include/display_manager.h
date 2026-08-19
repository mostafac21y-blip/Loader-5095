#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL.h>

struct Display {
    int index;
    SDL_Rect bounds;
    float scale;
    bool is_primary;
};

class DisplayManager {
public:
    DisplayManager();
    ~DisplayManager();

    // Initialize display system
    bool initialize();

    // Get number of displays
    int get_display_count() const;

    // Get display information
    const Display& get_display(int index) const;

    // Get all displays
    const std::vector<Display>& get_all_displays() const;

    // Get primary display
    const Display& get_primary_display() const;

    // Get total spanning area for all displays
    SDL_Rect get_total_bounds() const;

private:
    std::vector<Display> displays;
    int primary_display_index;

    void enumerate_displays();
};
