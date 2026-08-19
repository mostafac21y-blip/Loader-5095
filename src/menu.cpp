#include "menu.h"
#include <iostream>
#include <GL/gl.h>

Menu::Menu()
    : current_state(MenuState::MAIN), window(nullptr), menu_width(800),
      menu_height(600), confirmed(false), selected_index(0) {}

Menu::~Menu() {}

bool Menu::initialize(SDL_Window* window, int width, int height) {
    this->window = window;
    this->menu_width = width;
    this->menu_height = height;

    init_display_options();
    init_mode_options();
    init_fps_options();

    std::cout << "Menu system initialized" << std::endl;
    return true;
}

void Menu::set_state(MenuState state) {
    current_state = state;
    selected_index = 0;
}

MenuState Menu::get_state() const {
    return current_state;
}

void Menu::handle_input(SDL_KeyboardEvent& event) {
    if (event.type != SDL_KEYDOWN) {
        return;
    }

    switch (event.keysym.sym) {
        case SDLK_UP:
            selected_index = (selected_index > 0) ? selected_index - 1 : 0;
            break;
        case SDLK_DOWN: {
            std::vector<MenuItem>* items = nullptr;
            switch (current_state) {
                case MenuState::DISPLAY_SELECT:
                    items = &display_options;
                    break;
                case MenuState::MODE_SELECT:
                    items = &mode_options;
                    break;
                case MenuState::FPS_SELECT:
                    items = &fps_options;
                    break;
                default:
                    break;
            }
            if (items) {
                selected_index = (selected_index < static_cast<int>(items->size()) - 1)
                                    ? selected_index + 1
                                    : selected_index;
            }
            break;
        }
        case SDLK_RETURN:
            confirmed = true;
            break;
        case SDLK_ESCAPE:
            current_state = MenuState::MAIN;
            selected_index = 0;
            break;
        default:
            break;
    }
}

void Menu::render() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, menu_width, menu_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    switch (current_state) {
        case MenuState::MAIN:
            render_main_menu();
            break;
        case MenuState::DISPLAY_SELECT:
            render_display_select();
            break;
        case MenuState::MODE_SELECT:
            render_mode_select();
            break;
        case MenuState::FPS_SELECT:
            render_fps_select();
            break;
        default:
            break;
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

bool Menu::is_confirmed() const {
    return confirmed;
}

void Menu::reset() {
    confirmed = false;
}

int Menu::get_display_selection() const {
    if (selected_index < static_cast<int>(display_options.size())) {
        return display_options[selected_index].value;
    }
    return 0;
}

int Menu::get_mode_selection() const {
    if (selected_index < static_cast<int>(mode_options.size())) {
        return mode_options[selected_index].value;
    }
    return 0;
}

int Menu::get_fps_selection() const {
    if (selected_index < static_cast<int>(fps_options.size())) {
        return fps_options[selected_index].value;
    }
    return 60;
}

void Menu::update_display_options(int display_count) {
    display_options.clear();
    for (int i = 0; i < display_count; ++i) {
        MenuItem item;
        item.text = "Display " + std::to_string(i);
        item.value = i;
        item.selected = (i == 0);
        display_options.push_back(item);
    }
}

void Menu::update_fps_options(const std::vector<int>& fps_values) {
    fps_options.clear();
    for (int fps : fps_values) {
        MenuItem item;
        item.text = std::to_string(fps) + " FPS";
        item.value = fps;
        item.selected = false;
        fps_options.push_back(item);
    }
}

void Menu::render_main_menu() {
    render_menu_item("=== LOADER 5095 ===", menu_width / 2 - 100, 50, false);
    render_menu_item("", menu_width / 2 - 100, 100, false);
    render_menu_item("1. Select Display", menu_width / 2 - 100, 150,
                    selected_index == 0);
    render_menu_item("2. Select Mode", menu_width / 2 - 100, 200,
                    selected_index == 1);
    render_menu_item("3. Select FPS", menu_width / 2 - 100, 250,
                    selected_index == 2);
    render_menu_item("4. Settings", menu_width / 2 - 100, 300,
                    selected_index == 3);
    render_menu_item("5. Check Updates", menu_width / 2 - 100, 350,
                    selected_index == 4);
    render_menu_item("6. Start Loader", menu_width / 2 - 100, 400,
                    selected_index == 5);
    render_menu_item("", menu_width / 2 - 100, 450, false);
    render_menu_item("Press UP/DOWN to navigate, ENTER to select",
                    menu_width / 2 - 200, 500, false);
}

void Menu::render_display_select() {
    render_menu_item("=== SELECT DISPLAY ===", menu_width / 2 - 100, 50, false);
    int y = 150;
    for (size_t i = 0; i < display_options.size(); ++i) {
        render_menu_item(display_options[i].text, menu_width / 2 - 100, y,
                        static_cast<int>(i) == selected_index);
        y += 50;
    }
}

void Menu::render_mode_select() {
    render_menu_item("=== SELECT MODE ===", menu_width / 2 - 100, 50, false);
    int y = 150;
    for (size_t i = 0; i < mode_options.size(); ++i) {
        render_menu_item(mode_options[i].text, menu_width / 2 - 100, y,
                        static_cast<int>(i) == selected_index);
        y += 50;
    }
}

void Menu::render_fps_select() {
    render_menu_item("=== SELECT FPS ===", menu_width / 2 - 100, 50, false);
    int y = 150;
    for (size_t i = 0; i < fps_options.size(); ++i) {
        render_menu_item(fps_options[i].text, menu_width / 2 - 100, y,
                        static_cast<int>(i) == selected_index);
        y += 50;
    }
}

void Menu::render_menu_item(const std::string& text, int x, int y, bool selected) {
    // Placeholder for text rendering
    // In a real implementation, this would use a font library like FreeType
    if (selected) {
        std::cout << "> " << text << std::endl;
    } else {
        std::cout << "  " << text << std::endl;
    }
}

void Menu::init_display_options() {
    MenuItem item1;
    item1.text = "Display 0";
    item1.value = 0;
    item1.selected = true;
    display_options.push_back(item1);
}

void Menu::init_mode_options() {
    MenuItem modes[] = {
        {"Single Display", 0, true},
        {"Multi-Display", 1, false},
        {"Fullscreen", 2, false},
        {"Windowed", 3, false}
    };
    for (const auto& mode : modes) {
        mode_options.push_back(mode);
    }
}

void Menu::init_fps_options() {
    int fps_values[] = {30, 60, 120, 144, 240};
    for (int fps : fps_values) {
        MenuItem item;
        item.text = std::to_string(fps) + " FPS";
        item.value = fps;
        item.selected = (fps == 60);
        fps_options.push_back(item);
    }
}
