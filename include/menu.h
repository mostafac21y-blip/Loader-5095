#pragma once

#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "config_manager.h"

enum class MenuState {
    MAIN,
    DISPLAY_SELECT,
    MODE_SELECT,
    FPS_SELECT,
    SETTINGS,
    UPDATING,
    RUNNING
};

struct MenuItem {
    std::string text;
    int value;
    bool selected;
};

class Menu {
public:
    Menu();
    ~Menu();
    
    // Initialize menu system
    bool initialize(SDL_Window* window, int width, int height);
    
    // Set current menu state
    void set_state(MenuState state);
    MenuState get_state() const;
    
    // Handle menu input
    void handle_input(SDL_KeyboardEvent& event);
    
    // Render menu
    void render();
    
    // Get menu selections
    int get_display_selection() const;
    int get_mode_selection() const;
    int get_fps_selection() const;
    
    // Update menu items
    void update_display_options(int display_count);
    void update_fps_options(const std::vector<int>& fps_values);
    
    // Confirm and exit menu
    bool is_confirmed() const;
    void reset();
    
private:
    MenuState current_state;
    SDL_Window* window;
    int menu_width, menu_height;
    bool confirmed;
    int selected_index;
    
    // Menu items
    std::vector<MenuItem> display_options;
    std::vector<MenuItem> mode_options;
    std::vector<MenuItem> fps_options;
    
    // Render specific menus
    void render_main_menu();
    void render_display_select();
    void render_mode_select();
    void render_fps_select();
    void render_menu_item(const std::string& text, int x, int y, bool selected);
    
    // Initialize menu options
    void init_display_options();
    void init_mode_options();
    void init_fps_options();
};
